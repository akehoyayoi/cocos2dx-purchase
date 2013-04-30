//
//  ProductInfo.cpp
//  cocos2dx-purchase
//
//  Created by Yohei Okaya on 2013/03/14.
//  Copyright (c) 2013 , Yohei Okaya All rights reserved.
//

#include "ProductInfo.h"

NS_CC_PURCHASE_BEGIN

ProductInfo::ProductInfo(const string& productId ,
                         const string& name ,
                         int price)
: m_productId(productId)
, m_name(name)
, m_price(price){}

PurchaseSuccessResult::PurchaseSuccessResult(const string& productId ,
                                             long transactionDate)
: m_productId(productId)
, m_transactionDate(transactionDate){}

PurchaseFailedResult::PurchaseFailedResult(const string& productId ,
                                           int errorCode ,
                                           const string& errorDescription)
:m_productId(productId)
,m_errorCode(errorCode)
,m_errorDescription(errorDescription){}

NS_CC_PURCHASE_END
