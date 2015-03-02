//
//  StoreDatabase.cpp
//  cocos2dx-purchase
//
//  Created by Yohei Okaya on 2013/03/14.
//  Copyright (c) 2013 , Yohei Okaya All rights reserved.
//

#include "StoreDatabase.h"

#define DBPATH "data.sqlite"

NS_CC_PURCHASE_BEGIN

CCString* getDatabaseFilePath(CCString* prefix)
{
    CCFileUtils* pUtils = CCFileUtils::sharedFileUtils();
    const char* dbPath = prefix->length() == 0 ? DBPATH: CCString::createWithFormat("%s_%s", prefix->getCString(), DBPATH)->getCString();
    std::string documentsDir = pUtils->getWritablePath(false);
    return CCString::createWithFormat("%s%s", documentsDir.c_str(), dbPath);
}

CppSQLite3DB* StoreDatabase::prepare()
{
    return StoreDatabase::prepare(ccs(""));
}

CppSQLite3DB* StoreDatabase::prepare(const char *dbName)
{
    return StoreDatabase::prepare(ccs(dbName));
}

CppSQLite3DB* StoreDatabase::prepare(CCString *dbName)
{
    CppSQLite3DB* db = new CppSQLite3DB();
    if(db) {
        CCString *dbPath = getDatabaseFilePath(dbName);
    	try {
    		db->open(dbPath->getCString());
        } catch (CppSQLite3Exception &e) {
            CCLOG("%d : %s", e.errorCode(), e.errorMessage());
        }
        // create statement instance for sql queries/statements
        return db;
    }
    CC_SAFE_DELETE(db);
    return NULL;
}

CppSQLite3Query* StoreDatabase::execute(CppSQLite3DB* db, CCString* sql)
{
    CppSQLite3Query* query = new CppSQLite3Query();
    try {
        *query = db->execQuery(sql->getCString());
    } catch (CppSQLite3Exception &e) {
        CCLOG("%d : %s", e.errorCode(), e.errorMessage());
        query->finalize();
        CC_SAFE_DELETE(query);
        query = NULL;
    }
    return query;
}

bool StoreDatabase::executeUpdate(CppSQLite3DB* db, CCString* sql)
{
    int nRet = 0;
    try {
        nRet = db->execDML(sql->getCString());
    } catch (CppSQLite3Exception &e) {
        CCLOG("%d : %s", e.errorCode(), e.errorMessage());
    }
    return nRet > 0;
}

NS_CC_PURCHASE_END
