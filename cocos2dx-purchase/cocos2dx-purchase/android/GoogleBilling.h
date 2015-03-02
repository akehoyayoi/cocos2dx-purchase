#pragma once

namespace GoogleBilling
{
	typedef void (*actionFinished)(int);

//    typedef void (*paymentTransaction)(const char*, const char*,
//                                       const char*, int);
/*
     * Messages
     */

	static const int INIT_BILLING_NO = 0;
	static const int INIT_BILLING_YES = 1;
	static const int PURCHASE_SUCCESS = 2;
	static const int PURCHASE_ALREADY_PURCHASED = 3;
	static const int PURCHASE_FAIL = 4;
	static const int PURCHASE_USER_CANCELED =5;
	static const int CONSUME_SUCCESS = 6;
	static const int CONSUME_FAIL = 7;
    static const int RESTORERECEIPT_SUCCESS = 8;
    static const int RESTORERECEIPT_FAIL = 9;

    struct Billing
    {

        static bool init(actionFinished callback);
        static void purchase(const char* item, int amount, actionFinished callback);
        static void consume(const char* purchaseData, const char* signature, actionFinished callback);
        static void consumeOwnItem(const char* productId, actionFinished callback);
        static void restoreReceipt(actionFinished callback);
    };
};
