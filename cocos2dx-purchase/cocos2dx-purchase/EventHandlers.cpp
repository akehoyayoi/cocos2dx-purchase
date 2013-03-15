//
//  EventHandlers.cpp
//  cocos2dx-purchase
//
//  Created by Yohei Okaya on 2013/03/14.
//  Copyright (c) 2013 , Yohei Okaya All rights reserved.
//

#include "EventHandlers.h"

NS_CC_PURCHASE_BEGIN

EventHandlers* EventHandlers::m_instace = NULL;

void EventHandlers::addHandler(IEventHandler* handler) {
	handlers.insert(handler);
}

void EventHandlers::removeHandler(IEventHandler* handler) {
	handlers.erase(handler);
}

void EventHandlers::successPurchase(PurchaseSuccessResult& purchaseResult){
	set<IEventHandler*>::iterator itr;
	for ( itr = handlers.begin(); itr != handlers.end(); itr++ )
	    (*itr)->successPurchase(purchaseResult);
}

void EventHandlers::failedPurchase(PurchaseFailedResult& purchaseResult) {
	set<IEventHandler*>::iterator itr;
	for ( itr = handlers.begin(); itr != handlers.end(); itr++ )
	    (*itr)->failedPurchase(purchaseResult);
}

void EventHandlers::successServerProcess(void* data) {
	set<IEventHandler*>::iterator itr;
	for ( itr = handlers.begin(); itr != handlers.end(); itr++ )
	    (*itr)->successServerProcess(data);
}

void EventHandlers::failedServerProcess(void* data) {
	set<IEventHandler*>::iterator itr;
	for ( itr = handlers.begin(); itr != handlers.end(); itr++ )
	    (*itr)->failedServerProcess(data);
}

NS_CC_PURCHASE_END