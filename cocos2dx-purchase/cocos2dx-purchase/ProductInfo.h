//
//  ProductInfo.h
//  cocos2dx-purchase
//
//  Created by Yohei Okaya on 2013/03/14.
//  Copyright (c) 2013 , Yohei Okaya All rights reserved.
//

#ifndef __cocos2dx_purchase__ProductInfo__
#define __cocos2dx_purchase__ProductInfo__

#include "cocos2d.h"
#include <string>
#include "PurchaseMacros.h"

using namespace std;

NS_CC_PURCHASE_BEGIN

class ProductInfo {
private:
    string m_productId;
    string m_name;
    int m_price;
public:
    ProductInfo(const string& productId ,
                const string& name ,
                int price);
    inline string productId() { return this->m_productId; }
    inline string name() { return this->m_name; }
    inline int price() { return this->m_price; }

};

class PurchaseSuccessResult {
private:
    string m_productId;
    string m_transactionId;
    int m_transactionState;
    string m_transactionReceipt;
public:
    PurchaseSuccessResult(const string& productId ,
                          const string& transactionId ,
                          int transactionState ,
                          const string& transationReceipt);
    inline string productId() { return this->m_productId; }
    inline string transactionId() { return this->m_transactionId; }
    inline int transactionState() { return this->m_transactionState; }
    inline string transactionReceipt() { return this->m_transactionReceipt; }
};

class PurchaseFailedResult {
private:
    string m_productId;
    string m_transactionId;
    int m_transactionState;
    int m_errorCode;
    string m_errorDescription;
public:
    PurchaseFailedResult(const string& productId ,
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

#endif /* defined(__cocos2dx_purchase__ProductInfo__) */
