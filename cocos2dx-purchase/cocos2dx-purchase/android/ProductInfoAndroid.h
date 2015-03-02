//
//  ProductInfoAndroid.h
//  cocos2dx-purchase
//
//  Created by Yohei Okaya on 2013/03/14.
//  Copyright (c) 2013 , Yohei Okaya All rights reserved.
//

#ifndef __cocos2dx_purchase__ProductInfoAndroid__
#define __cocos2dx_purchase__ProductInfoAndroid__

#include "cocos2d.h"
#include <string>
#include "PurchaseMacros.h"
#include "ProductInfo.h"

using namespace std;

NS_CC_PURCHASE_BEGIN

class PurchaseSuccessResultAndroid : public PurchaseSuccessResult {
protected:
    string m_purchaseData;
    string m_signature;
    int m_purchaseState;
public:
    PurchaseSuccessResultAndroid(const string& productId ,
                                const string& purchaseData,
                                const string& signature,
                                int purchaseState,
                                long transactionDate);
    inline string purchaseData() { return this->m_purchaseData; }
    inline string signature() { return this->m_signature; }
    inline int purchaseState() { return this->m_purchaseState; }
};

class PurchaseFailedResultAndroid: public PurchaseFailedResult {
protected:
    string m_purchaseData;
    string m_signature;
public:
    PurchaseFailedResultAndroid(const string& productId ,
                               int errorCode ,
                               const string& errorDescription,
                               const string& purchaseData,
                               const string& signature);
    inline string purchaseData() { return this->m_purchaseData; }
    inline string signature() { return this->m_signature; }
};

NS_CC_PURCHASE_END

#endif /* defined(__cocos2dx_purchase__ProductInfoAndroid__) */
