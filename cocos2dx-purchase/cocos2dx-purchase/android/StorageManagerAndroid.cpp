//
//  StorageManager.cpp
//  cocos2dx-purchase
//
//  Created by Yohei Okaya on 2013/03/14.
//  Copyright (c) 2013 , Yohei Okaya All rights reserved.
//

#include "StorageManagerAndroid.h"

#define PURCHASE_INFO "purchase_info"

NS_CC_PURCHASE_BEGIN

StorageManager* StorageManager::m_instance = NULL;

void StorageManager::createDatabase(){
    // create purchase transaction table
    if(StoreDatabase::executeUpdate(PURCHASE_INFO, "CREATE TABLE IF NOT EXISTS purchase_info("
                                    "product_id TEXT , "
                                    "purchase_data TEXT , "
                                    "signature TEXT , "
                                    "purchase_state INTEGER , "
                                    "transaction_date INTEGER)") == false){
        CCLOG("already : purchase_info migrate");
    }
}



void StorageManager::storePurchase(string& productId,
                                   string& purchaseData,
                                   string& signature,
                                   int purchaseState){
    // delete purcahse_info
    this->deletePurchase();

    // insert purchase_info
    this->insertPurchase(productId, purchaseData, signature, purchaseState);
}

void StorageManager::insertPurchase(string& productId,
                                    string& purchaseData,
                                    string& signature,
                                    int purchaseState){
    // insert purchase_info
    time_t t;
    time(&t);
    CCString* sql = CCString::createWithFormat("insert or replace into purchase_info("
    		                                   "product_id, "
                                               "purchase_data,"
                                               "signature, "
    		                                   "purchase_state, "
                                               "transaction_date) values ('%s','%s','%s','%d','%ld')",
                                               productId.c_str(),
                                               purchaseData.c_str(),
                                               signature.c_str(),
                                               purchaseState,
                                               t);
    if(StoreDatabase::executeUpdate(PURCHASE_INFO, sql->getCString())) {
        CCLOG("success : insert purchase_info");
    } else {
        CCLOG("failed : insert purchase_info");
    }
}

void StorageManager::deletePurchase(){
    // delete purcahse_info
    if(StoreDatabase::executeUpdate(PURCHASE_INFO, "delete from purchase_info")){
        CCLOG("success : delete purchase_info");
    } else {
        CCLOG("failed : delete purchase_info");
    }
}

PurchaseSuccessResultAndroid StorageManager::getPurchase(){
    CppSQLite3Query* rs = StoreDatabase::execute(PURCHASE_INFO, "select * from purchase_info");
    if(rs == NULL) return PurchaseSuccessResultAndroid("","","",-1,0);
    if(rs->eof()) return PurchaseSuccessResultAndroid("","","",-1,0);
    PurchaseSuccessResultAndroid result(strForColumn(rs, "product_id")->getCString(),
    		                            strForColumn(rs, "purchase_data")->getCString(),
                                        strForColumn(rs, "signature")->getCString(),
                                        intForColumn(rs, "purchase_state"),
                                        longForColumn(rs, "transaction_date"));
    CC_SAFE_DELETE(rs);
    return result;
}

NS_CC_PURCHASE_END
