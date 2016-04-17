# C++11 support is enabled

LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES:= ../service_api/service_api_impl.cpp pal.cpp

LOCAL_SHARED_LIBRARIES := libc++ libutils liblog libcutils libbinder

OMADM_C_INCLUDES := $(LOCAL_PATH)/../../../frameworks/base/include/ \
                    $(LOCAL_PATH)/../../../system/core/include/ \
                    $(LOCAL_PATH)/../../../external/libcxx/include/ \

LOCAL_C_INCLUDES := $(LOCAL_PATH) $(LOCAL_PATH)/../include
LOCAL_C_INCLUDES += $(OMADM_C_INCLUDES)

LOCAL_CLANG := true
LOCAL_SHARE_LIBRARIES := libbinder
LOCAL_CPPFLAGS := \
        -std=c++11 \

LOCAL_MODULE:= libpal
LOCAL_MODULE_OWNER := verizon

include $(BUILD_SHARED_LIBRARY)