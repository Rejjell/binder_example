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

LOCAL_MODULE_TAGS := optional

LOCAL_MODULE:= OmadmServer

include $(BUILD_EXECUTABLE)