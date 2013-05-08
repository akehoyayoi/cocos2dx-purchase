
#include "InAppPurchaseManager.h"
#include "EventHandlers.h"
#include "PurchaseMacros.h"
#include "StorageManagerAndroid.h"
#include "GoogleBilling.h"
#include "ProductInfo.h"

NS_CC_PURCHASE_BEGIN

CCString* InAppPurchaseManager::m_productId = NULL;
int InAppPurchaseManager::m_price = 0;
bool InAppPurchaseManager::m_retry = false;
BillingServiceInit InAppPurchaseManager::m_init = BillingServiceDisconnected;

InAppPurchaseManager& InAppPurchaseManager::getInstance()
{
   static InAppPurchaseManager _manager;
   return _manager;
}

bool InAppPurchaseManager::purchase(CCString * productId, int price)
{
    CC_SAFE_RELEASE(m_productId);
    m_productId = productId;
    CC_SAFE_RETAIN(m_productId);
	m_productId = productId;
	m_price = price;
	m_retry = false;

	StorageManager* storageManager = StorageManager::getInstance();
    PurchaseSuccessResultAndroid result = storageManager->getPurchase();
    int purchaseState = result.purchaseState();
    //CCLOG("purchaseState = %d", purchaseState);

    if(purchaseState == SKPaymentTransactionStatePurchased){
        EventHandlers::getInstance()->successPurchase(&result);
        CCLOG("previous purchase success");
        return true;
    } else if(purchaseState > 0) {
        CCLOG("previous purchase failed");
        return false;
    }

    if(m_init == BillingServiceDisconnected) {
		m_init = BillingServiceConnecting;
		GoogleBilling::Billing::init(InAppPurchaseManager::BillingInitHandler);
	} else if(m_init == BillingServiceConnected) {
		purchaseMain();
	}
    return true;
}

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

    CC_SAFE_RELEASE(m_productId);
}

void InAppPurchaseManager::BillingInitHandler(int error)
{
	if (error == GoogleBilling::INIT_BILLING_YES) {
		m_init = BillingServiceConnected;
		CCLog("Billing initialised");
		purchaseMain();
	}
	else if (error == GoogleBilling::INIT_BILLING_NO) {
		m_init = BillingServiceDisconnected;
		CCLog("Billing not availalble!");
	}
}

void InAppPurchaseManager::PurchasedHandler(int result) {
	if (result == GoogleBilling::PURCHASE_ALREADY_PURCHASED) {
		CCLog("Shop:Item already purchased!");
		if(!m_retry) {
			m_retry = true;
			GoogleBilling::Billing::consumeOwnItem(m_productId->getCString(), InAppPurchaseManager::ConsumeOwnItemtHandler);
		}
	} else if (result == GoogleBilling::PURCHASE_USER_CANCELED) {
		CCLog("Shop: Purchase user canceled!");
	    // レコード削除
	    StorageManager::getInstance()->deletePurchase();
	} else if (result == GoogleBilling::PURCHASE_FAIL) {
		CCLog("Shop: Purchase failed!");
        PurchaseFailedResultAndroid result(string("") , 99999 , string("purchase failed"), string(""), string(""));
        EventHandlers::getInstance()->failedPurchase(&result);
	} else if (result == GoogleBilling::PURCHASE_SUCCESS) {
		CCLog("Shop: Purchasing done");
	}
}

void InAppPurchaseManager::ConsumeOwnItemtHandler(int result)
{
	if (result == GoogleBilling::CONSUME_SUCCESS) {
		purchaseMain();
	}
}

bool InAppPurchaseManager::paymentTransaction(const char* productId, const char* purchaseData, const char* signature, int purchaseState)
{
	string strProductId(productId);
	string strPurchaseData(purchaseData);
	string strSignature(signature);
	if(purchaseState == SKPaymentTransactionStatePurchased) { // success purchase
		// store success purchase transaction
        StorageManager::getInstance()->storePurchase(strProductId, strPurchaseData, strSignature, purchaseState);
		PurchaseSuccessResultAndroid* result =
				new PurchaseSuccessResultAndroid(strProductId, strPurchaseData, strSignature, purchaseState, 0);
		EventHandlers::getInstance()->successPurchase(result);
	} else if(purchaseState == SKPaymentTransactionStateFailed) { // failed purchase
		int errorCode = 0;
        string errorDescription = "";
        PurchaseFailedResult* result =
        		new PurchaseFailedResultAndroid(strProductId, errorCode, errorDescription,
        				strPurchaseData, strSignature);
		EventHandlers::getInstance()->failedPurchase(result);
	} else if(purchaseState == SKPaymentTransactionStateRestored) { // not support Non-Cosumable item
		// do nothing
	}
}

void InAppPurchaseManager::consume(const char* purchaseData, const char* signature)
{
//	return GoogleBilling::Billing::consume(purchaseData, signature, InAppPurchaseManager::ConsumeHandler);
}

void InAppPurchaseManager::ConsumeHandler(int result) {
}

NS_CC_PURCHASE_END
