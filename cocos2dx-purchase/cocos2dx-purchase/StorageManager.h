//
//  StorageManager.h
//  cocos2dx-purchase
//
//  Created by OKAYA YOHEI on 13/03/19.
//
//

#ifndef __cocos2dx_purchase__StorageManager__
#define __cocos2dx_purchase__StorageManager__

#include "cocos2d.h"
#include <string>
#include "StoreDatabase.h"
#include "ProductInfo.h"

USING_NS_CC;
USING_NS_CC_PURCHASE;
using namespace std;

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

    PurchaseSuccessResult getPurchase();
    
    
private:
    static StorageManager* m_instance;
    StorageManager(){}
    StorageManager(const StorageManager& rhs);
    StorageManager& operator=(const StorageManager& rhs);
};


#endif /* defined(__cocos2dx_purchase__StorageManager__) */
