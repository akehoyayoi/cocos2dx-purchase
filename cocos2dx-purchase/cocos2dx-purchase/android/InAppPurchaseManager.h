#ifndef INAPPPURCHASEMANAGER_H
#define INAPPPURCHASEMANAGER_H
//
//  InAppPurchaseManager.h
//  cocos2dx-purchase
//
//  Created by Yohei Okaya on 2013/03/14.
//  Copyright (c) 2013 , Yohei Okaya All rights reserved.
//

#include "cocos2d.h"
#include "PurchaseMacros.h"

using namespace std;

NS_CC_PURCHASE_BEGIN

typedef enum {
    SKPaymentTransactionStatePurchased,     // Transaction is in queue, user has been charged.  Client should complete the transaction.
    SKPaymentTransactionStateFailed,        // Transaction was cancelled or failed before being added to the server queue.
    SKPaymentTransactionStateRestored       // Transaction was restored from user's purchase history.  Client should complete the transaction.
} SKPaymentTransactionState;

typedef enum {
    BillingServiceDisconnected,
    BillingServiceConnecting,
    BillingServiceConnected
} BillingServiceInit;

class InAppPurchaseManager
: public CCObject
{
public:
    static InAppPurchaseManager& getInstance();
    bool purchase(CCString * productId, int price);
    bool checkPreviousPurchase(bool *check);
    void paymentTransaction(const char* productId,
                            const char* purchaseData,
                            const char* signature,
                            int purchaseState);

protected:

    static void purchaseMain();
    static void consume();
    static void restoreReceipt();
    static void BillingInitHandler(int error);
    static void PurchasedHandler(int result);
    static void ConsumeOwnItemtHandler(int result);
    static void RestoreReceiptHandler(int result);
    static void successPurchase(const string& productId,
                         const string& purchaseData,
                         const string& signature,
                         int purchaseState);
    static void failedPurchase(const string& productId,
                        int errorCode,
                        const string& errorDescription,
                        const string& purchaseData,
                        const string& signature);

private:
    InAppPurchaseManager() {};
    ~InAppPurchaseManager() {};

    static CCString* m_productId;
    static int m_price;
    static BillingServiceInit m_init;
    static void (*m_next)(void);

    InAppPurchaseManager(const InAppPurchaseManager&);
    InAppPurchaseManager& operator=(const InAppPurchaseManager&);
};

NS_CC_PURCHASE_END

#endif

