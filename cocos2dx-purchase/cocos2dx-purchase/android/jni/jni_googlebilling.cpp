#include "../GoogleBilling.h"
#include "../InAppPurchaseManager.h"

#include "platform/android/jni/JniHelper.h"
#include "cocos2d.h"

using namespace cocos2d;

namespace GoogleBilling
{
    bool Billing::init(actionFinished callback)
	{
		JniMethodInfo t;
        if (JniHelper::getStaticMethodInfo(t,
                                           "com/geishatokyo/purchase/GoogleBilling",
                                           "init",
                                           "(J)Z"))
		{
			jlong arg = (long)(void*)callback;
            jboolean ret = t.env->CallStaticBooleanMethod(t.classID, t.methodID, arg);
			t.env->DeleteLocalRef(t.classID);
            return ret == JNI_TRUE;
		}
        return false;
	}


    void Billing::purchase(const char* key, int amount, actionFinished callback)
    {
        JniMethodInfo t;
        if (JniHelper::getStaticMethodInfo(t,
                                       "com/geishatokyo/purchase/GoogleBilling",
                                       "purchase",
                                       "(Ljava/lang/String;IJ)V"))
        {
            jstring arg = t.env->NewStringUTF(key);
            jlong arg2 = (long)(void*)callback;

            t.env->CallStaticVoidMethod(t.classID, t.methodID, arg,(jint) amount, arg2);
            t.env->DeleteLocalRef(arg);
            t.env->DeleteLocalRef(t.classID);
        }
    }

    void Billing::consume(const char* purchaseData, const char* signature, actionFinished callback)
    {
        JniMethodInfo t;
        if (JniHelper::getStaticMethodInfo(t,
                                           "com/geishatokyo/purchase/GoogleBilling",
                                           "consume",
                                           "(Ljava/lang/String;Ljava/lang/String;J)V"))
        {
            jstring arg = t.env->NewStringUTF(purchaseData);
            jstring arg2 = t.env->NewStringUTF(signature);
            jlong arg3 = (long)(void*)callback;

            t.env->CallStaticVoidMethod(t.classID, t.methodID, arg, arg2, arg3);
            t.env->DeleteLocalRef(arg);
            t.env->DeleteLocalRef(arg2);
            t.env->DeleteLocalRef(t.classID);
        }
    }
}

extern "C"
{
    void Java_com_geishatokyo_purchase_GoogleBilling_nativeactionFinished(JNIEnv* env, jobject thiz, jlong callback_address, jint error_code)
	{
		if (callback_address)
			(*reinterpret_cast<GoogleBilling::actionFinished>(callback_address))(error_code);
	}

    jboolean Java_com_geishatokyo_purchase_GoogleBilling_nativepaymentTransaction(JNIEnv* env, jobject thiz, jstring productId,
    		                                                                      jstring purchaseData, jstring signature, jint state)
	{
        const char *nativeProductId = env->GetStringUTFChars(productId, 0);
        const char *nativePurchaseData = env->GetStringUTFChars(purchaseData, 0);
        const char *nativeSignature = env->GetStringUTFChars(signature, 0);
        jboolean ret = purchase::InAppPurchaseManager::getInstance().paymentTransaction(nativeProductId, nativePurchaseData, nativeSignature, state);
        env->ReleaseStringUTFChars(productId, nativeProductId);
        env->ReleaseStringUTFChars(purchaseData, nativePurchaseData);
        env->ReleaseStringUTFChars(signature, nativeSignature);
        return ret;
	}
}
