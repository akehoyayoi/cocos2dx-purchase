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

class InAppPurchaseManager
: public CCObject
//, public SKProductsRequestDelegate
//, public SKPaymentTransactionObserver
{
public:
    InAppPurchaseManager();
    static InAppPurchaseManager* getInstance();
    bool purchase(CCString * productId);
//    void productsRequest(SKProductsRequest *request);
//    void paymentQueue(SKPaymentQueue *queue, CCArray *transactions);
//    
//protected:
//    SKProductsRequest * m_productRequest;
};

NS_CC_PURCHASE_END

#endif

