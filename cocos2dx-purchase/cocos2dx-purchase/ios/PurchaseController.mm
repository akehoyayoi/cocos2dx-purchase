//
//  PurchaseController.cpp
//  cocos2dx-purchase
//
//  Created by Yohei Okaya on 2013/03/14.
//  Copyright (c) 2013 , Yohei Okaya All rights reserved.
//

#include "PurchaseController.h"

#import "InAppPurchaseManager.h"

PurchaseController* PurchaseController::m_instace = NULL;

bool PurchaseController::purchase(ProductInfo& productIndo){
    NSString* productId = [[NSString alloc] initWithUTF8String:productIndo.productId().c_str()];
    return [[InAppPurchaseManager getInstance] purchase:productId];
}