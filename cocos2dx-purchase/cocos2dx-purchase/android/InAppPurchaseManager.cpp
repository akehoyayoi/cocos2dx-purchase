
#include "InAppPurchaseManager.h"
#include "EventHandlers.h"
#include "PurchaseMacros.h"
#include "StorageManagerAndroid.h"
#include "GoogleBilling.h"
#include "ProductInfo.h"
#include <string>

NS_CC_PURCHASE_BEGIN

bool InAppPurchaseManager::m_init = false;

InAppPurchaseManager& InAppPurchaseManager::getInstance()
{
   if(!m_init) {
	   m_init = true;
	   GoogleBilling::Billing::init(InAppPurchaseManager::BillingInitHandler);
   }

   static InAppPurchaseManager _manager;
   return _manager;
}

bool InAppPurchaseManager::purchase(CCString * productId, int price)
{

    StorageManager* storageManager = StorageManager::getInstance();
    PurchaseSuccessResultAndroid result = storageManager->getPurchase();
    int purchaseState = result.purchaseState();
    CCLOG("purchaseState = %d", purchaseState);

//    if(purchaseState == SKPaymentTransactionStatePurchased){
//        EventHandlers::getInstance()->successPurchase(&result);
//        CCLOG("previous purchase success");
//        return true;
//    } else if(purchaseState > 0) {
//        CCLOG("previous purchase failed");
//        return false;
//    }

    // record transaction for duplicate payment
    string product_id(productId->getCString());
    string purchase_data("");
    string signature("");
    storageManager->storePurchase(product_id, purchase_data, signature, 99999);

    GoogleBilling::Billing::purchase(productId->getCString(), price, InAppPurchaseManager::PurchasedHandler);

    return true;
}

void InAppPurchaseManager::BillingInitHandler(int error)
{
	if (error ==GoogleBilling::INIT_BILLING_YES) {
		CCLog("Billing initialised");
	}
	else if (error == GoogleBilling::INIT_BILLING_NO) {
		CCLog("Billing not availalble!");
	}
}

void InAppPurchaseManager::PurchasedHandler(int result) {
	if (result == GoogleBilling::PURCHASE_ALREADY_PURCHASED) {
		CCLog("Shop:Item already purchased!");
	} else if (result == GoogleBilling::PURCHASE_FAIL) {
		CCLog("Shop: Purchase failed or canceled!");
        PurchaseFailedResultAndroid result(string("") , 99999 , string("purchase failed"), string(""), string(""));
        EventHandlers::getInstance()->failedPurchase(&result);
	} else if (result == GoogleBilling::PURCHASE_SUCCESS) {
		CCLog("Shop: Purchasing done");
	}
}

bool InAppPurchaseManager::paymentTransaction(const char* productId, const char* purchaseData, const char* signature, int purchaseState)
{
	string strProductId = productId;
	string strPurchaseData = purchaseData;
	string strSignature = signature;
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
	return GoogleBilling::Billing::consume(purchaseData, signature, InAppPurchaseManager::ConsumeHandler);
}

void InAppPurchaseManager::ConsumeHandler(int result) {
}

NS_CC_PURCHASE_END
