LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := cocos2dxpurchase_static
                
LOCAL_MODULE_FILENAME := libcocos2dxpurchase

LOCAL_SRC_FILES := \
jni/jni_googlebilling.cpp \
PurchaseController.cpp \
InAppPurchaseManager.cpp \
StorageManagerAndroid.cpp \
ProductInfoAndroid.cpp \
../EventHandlers.cpp \
../ProductInfo.cpp \
../StoreDatabase.cpp \
#../libs/cppSQLite/CppSQLite3.cpp

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/..

LOCAL_C_INCLUDES := $(LOCAL_PATH)/.. \
                    $(LOCAL_PATH)/../libs/cppSQLite \
                    $(LOCAL_PATH)/../../../../cocos2d-x/cocos2dx \
                    $(LOCAL_PATH)/../../../../cocos2d-x/cocos2dx/include \
                    $(LOCAL_PATH)/../../../../cocos2d-x/cocos2dx/kazmath/include \
                    $(LOCAL_PATH)/../../../../cocos2d-x/cocos2dx/platform/android

$(info LOCAL_C_INCLUDES=$(LOCAL_C_INCLUDES))

PCH_FILE := $(LOCAL_PATH)/../../Prefix.pch

# 追加するC++のコンパイルフラグ
LOCAL_CPPFLAGS += -include $(PCH_FILE) -fexceptions

include $(BUILD_STATIC_LIBRARY)
