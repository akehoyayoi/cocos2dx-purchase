//
//  StorageManager.h
//  cocos2dx-purchase
//
//  Created by Yohei Okaya on 2013/03/14.
//  Copyright (c) 2013 , Yohei Okaya All rights reserved.
//

#ifndef __cocos2dx_purchase__StorageManager__
#define __cocos2dx_purchase__StorageManager__

#include "cocos2d.h"
#include <string>
#include "StoreDatabase.h"
#include "ProductInfoIOS.h"
#include "PurchaseMacros.h"


using namespace std;

NS_CC_PURCHASE_BEGIN

class StorageManager {
public:
    static StorageManager* getInstance(){
        if(m_instance == NULL){
            m_instance = new StorageManager();
            m_instance->createDatabase();
        }
        return m_instance;
    }
    void createDatabase();
    void storePurchase(string& productId,
                       string& transactionId,
                       int transactionState,
                       string& transactionReceipt);
    void insertPurchase(string& productId,
                        string& transactionId,
                        int transactionState,
                        string& transactionReceipt);
    void deletePurchase();

    PurchaseSuccessResultIOS getPurchase();
    
    
private:
    static StorageManager* m_instance;
    StorageManager(){}
    StorageManager(const StorageManager& rhs);
    StorageManager& operator=(const StorageManager& rhs);
};

NS_CC_PURCHASE_END

#endif /* defined(__cocos2dx_purchase__StorageManager__) */
