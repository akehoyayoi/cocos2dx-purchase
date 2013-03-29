LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := cocos2dxpurchase_static
                
LOCAL_MODULE_FILENAME := libcocos2dxpurchase

LOCAL_SRC_FILES := \
PurchaseController.cpp \
InAppPurchaseManager.cpp \
../EventHandlers.cpp \
../ProductInfo.cpp \
../StorageManager.cpp \
../StoreDatabase.cpp

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/..

LOCAL_C_INCLUDES := $(LOCAL_PATH)/.. \
                    $(LOCAL_PATH)/../libs/cppSQLite \
                    $(COCOS2DX_ROOT)/cocos2dx \
                    $(COCOS2DX_ROOT)/cocos2dx/include \
                    $(COCOS2DX_ROOT)/cocos2dx/kazmath/include \
                    $(COCOS2DX_ROOT)/cocos2dx/platform/android

PCH_FILE := $(LOCAL_PATH)/../../Prefix.pch

# 追加するC++のコンパイルフラグ
LOCAL_CPPFLAGS += -include $(PCH_FILE) -fexceptions

include $(BUILD_STATIC_LIBRARY)
