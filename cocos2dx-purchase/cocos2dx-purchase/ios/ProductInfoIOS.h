//
//  ProductInfoIOS.h
//  cocos2dx-purchase
//
//  Created by Yohei Okaya on 2013/03/14.
//  Copyright (c) 2013 , Yohei Okaya All rights reserved.
//

#ifndef __cocos2dx_purchase__ProductInfoIOS__
#define __cocos2dx_purchase__ProductInfoIOS__

#include "cocos2d.h"
#include <string>
#include "PurchaseMacros.h"
#include "ProductInfo.h"

using namespace std;

NS_CC_PURCHASE_BEGIN

class PurchaseSuccessResultIOS: public PurchaseSuccessResult {
private:
    string m_productId;
    string m_transactionId;
    int m_transactionState;
    string m_transactionReceipt;
    long m_transactionDate;
public:
    PurchaseSuccessResultIOS(const string& productId ,
                          const string& transactionId ,
                          int transactionState ,
                          const string& transationReceipt,
                          long transactionDate);
    inline string productId() { return this->m_productId; }
    inline string transactionId() { return this->m_transactionId; }
    inline int transactionState() { return this->m_transactionState; }
    inline string transactionReceipt() { return this->m_transactionReceipt; }
    inline long transactionDate() { return this->m_transactionDate; }
    inline long elapsedTime() {
        time_t t;
        time(&t);
        return (t - m_transactionDate);
    }
};

class PurchaseFailedResultIOS: public PurchaseFailedResult {
private:
    string m_productId;
    string m_transactionId;
    int m_transactionState;
    int m_errorCode;
    string m_errorDescription;
public:
    PurchaseFailedResultIOS(const string& productId ,
                         const string& transactionId ,
                         int transactionState ,
                         int errorCode ,
                            const string& errorDescription);
    inline string productId() { return this->m_productId; }
    inline string transactionId() { return this->m_transactionId; }
    inline int transactionState() { return this->m_transactionState; }
    inline int errorCode() { return this->m_errorCode; }
    inline string errorDescription() { return this->m_errorDescription; }
};

NS_CC_PURCHASE_END

#endif /* defined(__cocos2dx_purchase__ProductInfoIOS__) */
