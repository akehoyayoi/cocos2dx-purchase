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
    string m_transactionId;
    int m_transactionState;
    string m_transactionReceipt;
public:
    PurchaseSuccessResultIOS(const string& productId ,
                          const string& transactionId ,
                          int transactionState ,
                          const string& transationReceipt,
                          long transactionDate);
    inline string transactionId() { return this->m_transactionId; }
    inline int transactionState() { return this->m_transactionState; }
    inline string transactionReceipt() { return this->m_transactionReceipt; }
};

class PurchaseFailedResultIOS: public PurchaseFailedResult {
private:
    string m_transactionId;
    int m_transactionState;
public:
    PurchaseFailedResultIOS(const string& productId ,
                         const string& transactionId ,
                         int transactionState ,
                         int errorCode ,
                            const string& errorDescription);
    inline string transactionId() { return this->m_transactionId; }
    inline int transactionState() { return this->m_transactionState; }
};

NS_CC_PURCHASE_END

#endif /* defined(__cocos2dx_purchase__ProductInfoIOS__) */
