//
//  EventHandlers.h
//  cocos2dx-purchase
//
//  Created by Yohei Okaya on 2013/03/14.
//  Copyright (c) 2013 , Yohei Okaya All rights reserved.
//

#ifndef __cocos2dx_purchase__EventHandlers__
#define __cocos2dx_purchase__EventHandlers__

#include "cocos2d.h"
#include "ProductInfo.h"

class IEventHandler {
public:
    virtual void successPurchase(PurchaseSuccessResult& purchaseResult) = 0;
    virtual void failedPurchase(PurchaseFailedResult& purchaseResult) = 0;
    virtual void successServerProcess(void* data) {}
    virtual void failedServerProcess(void* data) {}
};

class EventHandlers {
public:
    static EventHandlers* getInstance(){
        if(m_instace == NULL){
            m_instace = new EventHandlers();
        }
        return m_instace;
    }
    
	void addHandler(IEventHandler* handler);
	void removeHandler(IEventHandler* handler);
    
    // this method is called by cocos2dx-purchase
    void successPurchase(PurchaseSuccessResult& purchaseResult);
    void failedPurchase(PurchaseFailedResult& purchaseResult);
    
    // if necessary this method is called by application
    void successServerProcess(void* data);
    void failedServerProcess(void* data);
    
private:
    static EventHandlers* m_instace;
	set <IEventHandler*> handlers;    
    EventHandlers(){}
    EventHandlers(const EventHandlers& rhs);
    EventHandlers& operator=(const EventHandlers& rhs);
};

#endif /* defined(__cocos2dx_purchase__EventHandlers__) */
