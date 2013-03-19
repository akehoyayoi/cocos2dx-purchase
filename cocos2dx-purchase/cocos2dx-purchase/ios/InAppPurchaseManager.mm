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
    if([SKPaymentQueue canMakePayments] == NO) {
        return NO;
    }
    
    // トランザクションのチェックを行う
    // 無ければ以降の処理を行う
    // 課金成功が残っていれば、成功処理を呼び出す
    // 課金成功以外が残っていれば、終了する
    
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
        PurchaseFailedResult result(string("") , string("") , 0 , 99999 , string("invalid response"));
        EventHandlers::getInstance()->failedPurchase(result);
        return;
    }
    
    if([[response products] count] == 0){
        PurchaseFailedResult result(string("") , string("") , 0 , 99999 , string("valid product is nothing"));
        EventHandlers::getInstance()->failedPurchase(result);
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
                // 課金成功をDBに記録する
                
                PurchaseSuccessResult result(productId,
                                             transactionId,
                                             transactionState,
                                             transationReceipt);
                EventHandlers::getInstance()->successPurchase(result);
                break;
            }
            case SKPaymentTransactionStateFailed: // failed purchase
            {
                [queue finishTransaction:transaction];
                int errorCode = [[transaction error] code];
                string errorDescription([[[transaction error] localizedDescription] UTF8String]);
                // トランザクションをパージする？
                
                PurchaseFailedResult result(productId,
                                            transactionId,
                                            transactionState,
                                            errorCode,
                                            errorDescription);
                EventHandlers::getInstance()->failedPurchase(result);
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
