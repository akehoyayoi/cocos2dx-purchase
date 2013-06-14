//
//  PurchaseController.cpp
//  cocos2dx-purchase
//
//  Created by Yohei Okaya on 2013/03/14.
//  Copyright (c) 2013 , Yohei Okaya All rights reserved.
//

#include "PurchaseController.h"

#import "InAppPurchaseManager.h"
#include "StorageManagerIOS.h"

NS_CC_PURCHASE_BEGIN

PurchaseController* PurchaseController::m_instance = NULL;

bool PurchaseController::checkPreviousPurchase(){
    BOOL check = NO;
    if([[InAppPurchaseManager getInstance] checkPreviousPurchase: &check]) {
        return check;
    }
    return true;
}
    
bool PurchaseController::purchase(ProductInfo& productIndo){
    NSString* productId = [[NSString alloc] initWithUTF8String:productIndo.productId().c_str()];
    return [[InAppPurchaseManager getInstance] purchase:productId];
}

bool PurchaseController::finishPurchase(){
    StorageManager::getInstance()->deletePurchase();
    return true;
}

NS_CC_PURCHASE_END