//
//  InAppPurchaseManager.m
//  cocos2dx-purchase
//
//  Created by Yohei Okaya on 2013/03/14.
//  Copyright (c) 2013 , Yohei Okaya All rights reserved.
//

#import "InAppPurchaseManager.h"
#import "NSData+Base64.h"
#include "EventHandlers.h"
#include "PurchaseMacros.h"
#include "StorageManagerIOS.h"
#include <string>

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(o) if (o!=nil) { [o release]; o=nil; }
#endif

using namespace std;
USING_NS_CC_PURCHASE;

@implementation InAppPurchaseManager

+ (InAppPurchaseManager*)getInstance
{
    static InAppPurchaseManager* _instance = nil;
    @synchronized( self ) {
        if( _instance == nil ) {
            _instance = [[InAppPurchaseManager alloc ] init];
        }
    }
    return _instance;
}

- (void)dealloc
{
    [[SKPaymentQueue defaultQueue] removeTransactionObserver:self];
    if (productRequest) {
        productRequest.delegate = nil;
        SAFE_RELEASE(productRequest);
    }
    [super dealloc];
}

- (BOOL)purchase:(NSString *) productId
{
    // check transaction
    // if transaction is nothing then continuerous process
    // success purchase transaction , success purchase process 
    // other , exit process
    StorageManager* storageManager = StorageManager::getInstance();
    PurchaseSuccessResultIOS result = storageManager->getPurchase();
    int transactionState = result.transactionState();
    if(transactionState == SKPaymentTransactionStatePurchased){
        EventHandlers::getInstance()->successPurchase(&result);
        CCLOG("previous purchase success");
        return YES;
    } else if(transactionState > 0) {
        CCLOG("previous purchase failed");
        return NO;
    }

    if([SKPaymentQueue canMakePayments] == NO) {
        return NO;
    }
    
    // record transaction for duplicate payment
    string product_id([productId UTF8String]);
    string transaction_id("");
    string transaction_receipt("");
    storageManager->storePurchase(product_id, transaction_id, 99999, transaction_receipt);
    
    if (productRequest) {
        productRequest.delegate = nil;
        SAFE_RELEASE(productRequest);
    }
    
    productRequest = [[SKProductsRequest alloc] initWithProductIdentifiers:[NSSet setWithObject:productId]];
    productRequest.delegate = self;
    [productRequest start];
    return YES;
}

- (void) productsRequest:(SKProductsRequest *)request
      didReceiveResponse:(SKProductsResponse *)response
{

    if(response == nil){
        PurchaseFailedResultIOS result(string("") , string("") , 0 , 99999 , string("invalid response"));
        EventHandlers::getInstance()->failedPurchase(&result);
        return;
    }
    
    if([[response products] count] == 0){
        PurchaseFailedResultIOS result(string("") , string("") , 0 , 99999 , string("valid product is nothing"));
        EventHandlers::getInstance()->failedPurchase(&result);
        return;
    }
    
    [[SKPaymentQueue defaultQueue] addTransactionObserver:self];
    for(SKProduct* product in response.products){
        SKPayment* payment = [SKPayment paymentWithProduct:product];
        [[SKPaymentQueue defaultQueue] addPayment:payment];
    }
}

- (void) paymentQueue:(SKPaymentQueue *)queue
  updatedTransactions:(NSArray *)transactions
{
    for(SKPaymentTransaction* transaction in transactions){
        string productId;
        if([transaction payment]){
            if([[transaction payment] productIdentifier]){
                productId = [[[transaction payment]productIdentifier] UTF8String];
            }
        }

        string transactionId;
        if([transaction transactionIdentifier]){
            transactionId = [[transaction transactionIdentifier] UTF8String];
        }
        
        int transactionState = [transaction transactionState];
        switch (transaction.transactionState) {
            case SKPaymentTransactionStatePurchased: // success purchase
            {
                [queue finishTransaction: transaction];
                string transationReceipt([[[transaction transactionReceipt] base64EncodedString] UTF8String]);
                // store success purchase transaction
                StorageManager::getInstance()->storePurchase(productId,
                                                             transactionId,
                                                             transactionState,
                                                             transationReceipt);
                PurchaseSuccessResultIOS result(productId,
                                             transactionId,
                                             transactionState,
                                             transationReceipt,0);
                EventHandlers::getInstance()->successPurchase(&result);
                break;
            }
            case SKPaymentTransactionStateFailed: // failed purchase
            {
                [queue finishTransaction:transaction];
                int errorCode = [[transaction error] code];
                string errorDescription([[[transaction error] localizedDescription] UTF8String]);
                PurchaseFailedResultIOS result(productId,
                                            transactionId,
                                            transactionState,
                                            errorCode,
                                            errorDescription);
                EventHandlers::getInstance()->failedPurchase(&result);
                break;
            }
            case SKPaymentTransactionStateRestored: // not support Non-Cosumable item
                break;
            default:
                break;
        }
    }
}


@end
