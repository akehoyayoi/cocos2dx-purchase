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

    struct Billing
    {

        static bool init(actionFinished callback);
        static void purchase(const char* item, int amount, actionFinished callback);
        static void consume(const char* purchaseData, const char* signature, actionFinished callback);
    };
};
