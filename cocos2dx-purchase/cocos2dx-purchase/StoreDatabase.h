//
//  StoreDatabase.h
//  cocos2dx-purchase
//
//  Created by Yohei Okaya on 2013/03/14.
//  Copyright (c) 2013 , Yohei Okaya All rights reserved.
//

#ifndef __cocos2dx_purchase__StoreDatabase__
#define __cocos2dx_purchase__StoreDatabase__

#include "cocos2d.h"
#include <sqlite3.h>
#include "CppSQLite3.h"

USING_NS_CC;



#define EXEC_SELECT_QUERY(sql, rs, func) EXEC_SELECT_QUERY_WITH_DB(NULL, sql, rs, func)

#define EXEC_SELECT_QUERY_WITH_DB(dbName, sql, rs, func) \
CppSQLite3DB* db = dbName == NULL ? DataAccessObject::prepare() : DataAccessObject::prepare((const char*)dbName);\
if (db) {\
rs = DataAccessObject::execute(db, ccs(sql));\
if (rs) {\
while(!rs->eof()) {\
func\
rs->nextRow();\
}\
}\
CC_SAFE_DELETE(rs);\
CC_SAFE_DELETE(db);\
}

inline CCString* strForColumn(CppSQLite3Query *rs, const char * columnName)
{
    return ccs(rs->getStringField(columnName));
}

inline int intForColumn(CppSQLite3Query *rs, const char *columnName)
{
    return rs->getIntField(columnName);
}

inline bool boolForColumn(CppSQLite3Query *rs, const char *columnName)
{
    return rs->getIntField(columnName) != 0;
}

inline bool doubleForColumn(CppSQLite3Query *rs, const char *columnName)
{
    return rs->getFloatField(columnName);
}

class StoreDatabase: public CCObject
{
public:
    static CppSQLite3DB* prepare();
    static CppSQLite3DB* prepare(const char* dbName);
    static CppSQLite3DB* prepare(CCString* dbName);
    
    static CppSQLite3Query* execute(CppSQLite3DB *db, CCString *sql);
    static bool executeUpdate(CppSQLite3DB *db, CCString *sql);
    static void execSelectQuery(const char *sql, void (*func)(CppSQLite3Query* rs)) {
        StoreDatabase::execSelectQuery(NULL, sql, func);
    }
    static void execSelectQuery(const char *dbName, const char *sql, void (*func)(CppSQLite3Query* rs)) {
        CppSQLite3DB* db = dbName == NULL ? StoreDatabase::prepare() : StoreDatabase::prepare(dbName);
        CppSQLite3Query* rs = StoreDatabase::execute(db, ccs(sql));
        while(!rs->eof()) {
            func(rs);
            rs->nextRow();
        }
        CC_SAFE_DELETE(db);
        CC_SAFE_DELETE(rs);
    }
    
    static bool executeUpdate(const char *sql) {
        return StoreDatabase::executeUpdate(NULL, sql);
    }
    static bool executeUpdate(const char *dbName, const char *sql) {
        CppSQLite3DB* db = dbName == NULL ? StoreDatabase::prepare() : StoreDatabase::prepare(dbName);
        bool result = StoreDatabase::executeUpdate(db, ccs(sql));
        CC_SAFE_DELETE(db);
        return result;
    }
};

#endif /* defined(__cocos2dx_purchase__StoreDatabase__) */
