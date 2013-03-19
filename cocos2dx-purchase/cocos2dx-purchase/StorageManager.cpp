//
//  StorageManager.cpp
//  cocos2dx-purchase
//
//  Created by Yohei Okaya on 2013/03/14.
//  Copyright (c) 2013 , Yohei Okaya All rights reserved.
//

#include "StorageManager.h"

#define PURCHASE_INFO "purchase_info"

StorageManager* StorageManager::m_instance = NULL;

void StorageManager::createDatabase(){
    // create purchase transaction table
    if(StoreDatabase::executeUpdate(PURCHASE_INFO, "CREATE TABLE IF NOT EXISTS purchase_info("
                                    "product_id TEXT , "
                                    "transaction_id TEXT , "
                                    "transaction_state INTEGER , "
                                    "transaction_receipt TEXT , "
                                    "transaction_date DOUBLE)") == false){
        CCLOG("already : purchase_info migrate");
    }
}



void StorageManager::storePurchase(string& productId,
                                   string& transactionId,
                                   int transactionState,
                                   string& transactionReceipt){
    // delete purcahse_info
    this->deletePurchase();
    
    // insert purchase_info
    this->insertPurchase(productId, transactionId, transactionState, transactionReceipt);
}

void StorageManager::insertPurchase(string& productId,
                                    string& transactionId,
                                    int transactionState,
                                    string& transactionReceipt){
    // insert purchase_info
    CCString* sql = CCString::createWithFormat("insert or replace into purchase_info("
                                               "product_id ,"
                                               "transaction_id , "
                                               "transaction_state , "
                                               "transaction_receipt , "
                                               "transaction_date) values ('%s','%s','%d','%s',datetime('now'))",
                                               productId.c_str(),
                                               transactionId.c_str(),
                                               transactionState,
                                               transactionReceipt.c_str());
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

PurchaseSuccessResult StorageManager::getPurchase(){
    CppSQLite3Query* rs = StoreDatabase::execute(PURCHASE_INFO, "select * from purchase_info");
    if(rs == NULL) return PurchaseSuccessResult("","",-1,"",0);
    if(rs->eof()) return PurchaseSuccessResult("","",-1,"",0);
    PurchaseSuccessResult result(strForColumn(rs, "product_id")->getCString(),
                                 strForColumn(rs, "transaction_id")->getCString(),
                                 intForColumn(rs, "transaction_state"),
                                 strForColumn(rs, "transaction_receipt")->getCString(),
                                 doubleForColumn(rs, "transaction_date"));
    CC_SAFE_DELETE(rs);
    return result;
}
