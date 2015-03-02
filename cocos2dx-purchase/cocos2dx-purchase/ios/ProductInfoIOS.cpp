//
//  ProductInfoIOS.cpp
//  cocos2dx-purchase
//
//  Created by Yohei Okaya on 2013/03/14.
//  Copyright (c) 2013 , Yohei Okaya All rights reserved.
//

#include "ProductInfoIOS.h"

NS_CC_PURCHASE_BEGIN

PurchaseSuccessResultIOS::PurchaseSuccessResultIOS(const string& productId ,
                                                   const string& transactionId ,
                                                   int transactionState ,
                                                   const string& transationReceipt,
                                                   long transactionDate)
: PurchaseSuccessResult(productId, transactionDate)
, m_transactionId(transactionId)
, m_transactionState(transactionState)
, m_transactionReceipt(transationReceipt){}

PurchaseFailedResultIOS::PurchaseFailedResultIOS(const string& productId ,
                                                 const string& transactionId ,
                                                 int transactionState ,
                                                 int errorCode ,
                                                 const string& errorDescription)
: PurchaseFailedResult(productId, errorCode, errorDescription)
, m_transactionId(transactionId)
, m_transactionState(transactionState){}

NS_CC_PURCHASE_END
