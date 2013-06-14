
#include "InAppPurchaseManager.h"
#include "EventHandlers.h"
#include "PurchaseMacros.h"
#include "StorageManagerAndroid.h"
#include "GoogleBilling.h"
#include "ProductInfo.h"

NS_CC_PURCHASE_BEGIN

CCString* InAppPurchaseManager::m_productId = NULL;
int InAppPurchaseManager::m_price = 0;
BillingServiceInit InAppPurchaseManager::m_init = BillingServiceDisconnected;
void (*InAppPurchaseManager::m_next)(void) = NULL;

InAppPurchaseManager& InAppPurchaseManager::getInstance()
{
   static InAppPurchaseManager _manager;
   return _manager;
}

bool InAppPurchaseManager::checkPreviousPurchase(bool *check)
{
	StorageManager* storageManager = StorageManager::getInstance();
    PurchaseSuccessResultAndroid result = storageManager->getPurchase();
    int purchaseState = result.purchaseState();
    //CCLOG("purchaseState = %d", purchaseState);

    if(purchaseState == SKPaymentTransactionStatePurchased){
        EventHandlers::getInstance()->successPurchase(&result);
        CCLOG("previous purchase success");
        *check = true;
        return true;
    } else if(purchaseState > 0) {
        // 購入情報が残っていれば、レシートを再作成
        CCLOG("previous purchase failed");
        if(m_init == BillingServiceDisconnected) {
            m_init = BillingServiceConnecting;
            m_next = &InAppPurchaseManager::restoreReceipt;
            GoogleBilling::Billing::init(InAppPurchaseManager::BillingInitHandler);
        } else if(m_init == BillingServiceConnected) {
            restoreReceipt();
        }
        *check = true;
        return true;
    }
    return false;
}

// 課金処理スタート
bool InAppPurchaseManager::purchase(CCString * productId, int price)
{
    bool check = false;
    if(checkPreviousPurchase(&check)) {
        return check;
    }

    CC_SAFE_RELEASE(m_productId);
    m_productId = productId;
    CC_SAFE_RETAIN(m_productId);
	m_price = price;

    if(m_init == BillingServiceDisconnected) {
		m_init = BillingServiceConnecting;
        m_next = &InAppPurchaseManager::consume;
		GoogleBilling::Billing::init(InAppPurchaseManager::BillingInitHandler);
	} else if(m_init == BillingServiceConnected) {
		consume();
	}
    return true;
}

// レシート再作成処理
void InAppPurchaseManager::restoreReceipt()
{
    GoogleBilling::Billing::restoreReceipt(InAppPurchaseManager::RestoreReceiptHandler);
}

// 消費処理
void InAppPurchaseManager::consume()
{
    GoogleBilling::Billing::consumeOwnItem(m_productId->getCString(), InAppPurchaseManager::ConsumeOwnItemtHandler);
}

// 課金処理メイン
void InAppPurchaseManager::purchaseMain()
{
	if(m_init != BillingServiceConnected) return;

	// record transaction for duplicate payment
	StorageManager* storageManager = StorageManager::getInstance();
    string product_id(m_productId->getCString());
    string purchase_data("");
    string signature("");
    storageManager->storePurchase(product_id, purchase_data, signature, 99999);

    GoogleBilling::Billing::purchase(m_productId->getCString(), m_price, InAppPurchaseManager::PurchasedHandler);
}

// 課金初期化処理のcallback
void InAppPurchaseManager::BillingInitHandler(int error)
{
	if (error == GoogleBilling::INIT_BILLING_YES) {
		m_init = BillingServiceConnected;
		CCLOG("Billing initialised");
		//consume();
        if(m_next) (*m_next)();
	}
	else if (error == GoogleBilling::INIT_BILLING_NO) {
		m_init = BillingServiceDisconnected;
		CCLOG("Billing not availalble!");
	}
}

// 消費処理のcallback
void InAppPurchaseManager::ConsumeOwnItemtHandler(int result)
{
	if (result == GoogleBilling::CONSUME_SUCCESS) {
        CCLOG("Consume success");
	} else {
        CCLOG("There is no need to be consume item");
    }
    purchaseMain();
}

// 課金処理のcallback
void InAppPurchaseManager::PurchasedHandler(int result) {
    string strProductId = m_productId->getCString();
	if (result == GoogleBilling::PURCHASE_ALREADY_PURCHASED) {
		CCLOG("Item already purchased!");
        failedPurchase(strProductId, 99999, string("purchase failed"), string(""), string(""));
	} else if (result == GoogleBilling::PURCHASE_USER_CANCELED) {
		CCLOG("Purchase user canceled!");
	    // レコード削除
	    StorageManager::getInstance()->deletePurchase();
        failedPurchase(strProductId, 99999, string("user canceled"), string(""), string(""));
	} else if (result == GoogleBilling::PURCHASE_FAIL) {
		CCLOG("Purchase failed!");
        failedPurchase(strProductId, 99999, string("purchase failed"), string(""), string(""));
	} else if (result == GoogleBilling::PURCHASE_SUCCESS) {
		CCLOG("Purchasing done");
	}
}

void InAppPurchaseManager::RestoreReceiptHandler(int result) {
	if (result == GoogleBilling::CONSUME_SUCCESS) {
        CCLOG("restore receipt success");
	} else {
        CCLOG("restore receipt failed");
    }
}

void InAppPurchaseManager::paymentTransaction(const char* productId,
                                              const char* purchaseData,
                                              const char* signature,
                                              int purchaseState)
{
    string strProductId = productId;
    string strPurchaseData = purchaseData;
    string strSignature = signature;
	if(purchaseState == SKPaymentTransactionStatePurchased) { // success purchase
		// store success purchase transaction
        StorageManager::getInstance()->storePurchase(strProductId, strPurchaseData, strSignature, purchaseState);
		successPurchase(strProductId, strPurchaseData, strSignature, purchaseState);
	} else if(purchaseState == SKPaymentTransactionStateFailed) { // failed purchase
		int errorCode = 0;
        string errorDescription = "";
        failedPurchase(strProductId, errorCode, errorDescription, strPurchaseData, strSignature);
	} else if(purchaseState == SKPaymentTransactionStateRestored) { // not support Non-Cosumable item
		// do nothing
	}
}

void InAppPurchaseManager::successPurchase(const string& productId,
                                           const string& purchaseData,
                                           const string& signature,
                                           int purchaseState) {
    PurchaseSuccessResultAndroid* result = new PurchaseSuccessResultAndroid(productId,
                                                                            purchaseData,
                                                                            signature,
                                                                            purchaseState,
                                                                            0);
    EventHandlers::getInstance()->successPurchase(result);
    CC_SAFE_RELEASE_NULL(m_productId);
}

void InAppPurchaseManager::failedPurchase(const string& productId,
                                          int errorCode,
                                          const string& errorDescription,
                                          const string& purchaseData,
                                          const string& signature) {
    PurchaseFailedResult* result = new PurchaseFailedResultAndroid(productId,
                                                                   errorCode,
                                                                   errorDescription,
                                                                   purchaseData,
                                                                   signature);
    EventHandlers::getInstance()->failedPurchase(result);
    CC_SAFE_RELEASE_NULL(m_productId);
}


NS_CC_PURCHASE_END
