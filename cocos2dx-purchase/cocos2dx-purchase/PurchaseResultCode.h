//
//  PurchaseResultCode.h
//  zoffy
//
//  Created by Tomohisa Kinomoto on 2015/03/02.
//
//

#ifndef zoffy_PurchaseResultCode_h
#define zoffy_PurchaseResultCode_h

typedef enum {
    kPurchaseSuccess,
    kPurchasePreviousSuccess,
    kPurchasePreviousFailed,
    kPurchaseCanNotMakePayments,
} PurchaseResultCode;

#endif
