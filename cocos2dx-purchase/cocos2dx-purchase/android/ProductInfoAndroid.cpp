//
//  ProductInfoAndroid.cpp
//  cocos2dx-purchase
//
//  Created by Yohei Okaya on 2013/03/14.
//  Copyright (c) 2013 , Yohei Okaya All rights reserved.
//

#include "ProductInfoAndroid.h"

NS_CC_PURCHASE_BEGIN

PurchaseSuccessResultAndroid::PurchaseSuccessResultAndroid(
                                            const string& productId ,
                                            const string& purchaseData,
                                            const string& signature,
                                            int purchaseState,
                                            long transactionDate)
: PurchaseSuccessResult(productId, transactionDate)
, m_purchaseData(purchaseData)
, m_signature(signature)
, m_purchaseState(purchaseState){}

PurchaseFailedResultAndroid::PurchaseFailedResultAndroid(
                                            const string& productId ,
                                            int errorCode ,
                                            const string& errorDescription,
                                            const string& purchaseData,
                                            const string& signature)
: PurchaseFailedResult(productId, errorCode, errorDescription)
, m_purchaseData(purchaseData)
, m_signature(signature){}

NS_CC_PURCHASE_END
