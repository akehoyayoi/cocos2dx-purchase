//
//  PurchaseController.cpp
//  cocos2dx-purchase
//
//  Created by Yohei Okaya on 2013/03/14.
//  Copyright (c) 2013 , Yohei Okaya All rights reserved.
//

#include "PurchaseController.h"

#include "InAppPurchaseManager.h"
#include "StorageManagerAndroid.h"
#include "ProductInfoAndroid.h"

NS_CC_PURCHASE_BEGIN

PurchaseController* PurchaseController::m_instance = NULL;

bool PurchaseController::purchase(ProductInfo& productInfo){
    CCString* productId = ccs(productInfo.productId().c_str());
    int price = productInfo.price();
    return InAppPurchaseManager::getInstance().purchase(productId, price);
}

bool PurchaseController::finishPurchase(){
    // レコード削除
    StorageManager::getInstance()->deletePurchase();
    return true;
}

NS_CC_PURCHASE_END
