//
//  InAppPurchaseManager.h
//  cocos2dx-purchase
//
//  Created by Yohei Okaya on 2013/03/14.
//  Copyright (c) 2013 , Yohei Okaya All rights reserved.
//

#import <Foundation/Foundation.h>
#import <StoreKit/StoreKit.h>

@interface InAppPurchaseManager  : NSObject<SKProductsRequestDelegate,SKPaymentTransactionObserver>{
    SKProductsRequest *productRequest;
}

+ (InAppPurchaseManager*)getInstance;

- (BOOL)purchase:(NSString *) productId;

@end
