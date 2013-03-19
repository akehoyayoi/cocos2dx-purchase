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

USING_NS_CC;
using namespace std;

class StorageManager {
    static StorageManager* getInstance(){
        if(m_instance == NULL){
            m_instance = new StorageManager();
        }
        return m_instance;
    }
    
    void storePurchase(string& productId,
                       string& transactionId,
                       int transactionState,
                       string& transactionReceipt);
    
private:
    static StorageManager* m_instance;
    StorageManager(){}
    StorageManager(const StorageManager& rhs);
    StorageManager& operator=(const StorageManager& rhs);
};


#endif /* defined(__cocos2dx_purchase__StorageManager__) */
