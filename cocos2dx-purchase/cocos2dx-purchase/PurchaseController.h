//
//  PurchaseController.h
//  cocos2dx-purchase
//
//  Created by Yohei Okaya on 2013/03/14.
//  Copyright (c) 2013 , Yohei Okaya All rights reserved.
//

#ifndef __cocos2dx_purchase__PurchaseController__
#define __cocos2dx_purchase__PurchaseController__

#include "cocos2d.h"
#include "ProductInfo.h"
#include "PurchaseMacros.h"
#include <exception>

NS_CC_PURCHASE_BEGIN

class PurchaseController {
public:
    // iOS + Android
    //   devide implementation
    //     iOS     : PurchaseController.mm
    //     Android : PurchaseController.cpp
    static PurchaseController* getInstance(){
        if(m_instace == NULL){
            m_instace = new PurchaseController();
        }
        return m_instace;
    }
    
    bool purchase(ProductInfo& productIndo);
    
private:
    static PurchaseController* m_instace;
    PurchaseController(){}
    PurchaseController(const PurchaseController& rhs);
    PurchaseController& operator=(const PurchaseController& rhs);
};

NS_CC_PURCHASE_END

#endif /* defined(__cocos2dx_purchase__PurchaseController__) */
