LOCAL_PATH := $(call my-dir)
libs := $(LOCAL_PATH)/../libs

LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := binder
LOCAL_SRC_FILES := $(libs)/libbinder.so
include $(PREBUILT_SHARED_LIBRARY)

LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := utils
LOCAL_SRC_FILES := $(libs)/libutils.so
include $(PREBUILT_SHARED_LIBRARY)

LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := cutils
LOCAL_SRC_FILES := $(libs)/libcutils.so
include $(PREBUILT_SHARED_LIBRARY)

LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES:= service_api_impl.cpp
#server.cpp main.cpp

LOCAL_SHARED_LIBRARIES := libc++ libutils libbinder liblog libcutils

OMADM_C_INCLUDES := $(LOCAL_PATH)/../../../frameworks/base/include/ \
                    $(LOCAL_PATH)/../../../system/core/include/ \
                    $(LOCAL_PATH)/../../../external/libcxx/include/ \

LOCAL_C_INCLUDES := $(LOCAL_PATH) $(LOCAL_PATH)/../include
LOCAL_C_INCLUDES += $(OMADM_C_INCLUDES)

LOCAL_CLANG := true

LOCAL_CPPFLAGS := \
        -std=c++11 \
#LOCAL_LDLIBS := -llog -lutils -llibc++ -lcutils -lbinder
LOCAL_MODULE_TAGS := optional

LOCAL_MODULE:= OmadmServer

include $(BUILD_EXECUTABLE)