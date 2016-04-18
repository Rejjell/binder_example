# C++11 support is enabled

LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES:= ../service_api/service_api_impl.cpp ../service_api/service_listner_impl.cpp pal.cpp

LOCAL_ALLOW_UNDEFINED_SYMBOLS := true
LOCAL_LDLIBS += -L$(LOCAL_PATH)/../libs
LOCAL_SHARED_LIBRARIES := libc++ libutils libbinder liblog libcutils

OMADM_C_INCLUDES := $(LOCAL_PATH)/../include \
                    $(LOCAL_PATH)/../../../../frameworks/base/include/ \
                    $(LOCAL_PATH)/../../../../system/core/include/ \
                    $(LOCAL_PATH)/../../../../external/libcxx/include/ \

LOCAL_C_INCLUDES := $(LOCAL_PATH) $(LOCAL_PATH)/../include
LOCAL_C_INCLUDES += $(OMADM_C_INCLUDES)

LOCAL_CLANG := true
LOCAL_CPPFLAGS += "HAVE_PTHREADS"
LOCAL_CPPFLAGS := \
        -std=c++11 \

LOCAL_MODULE:= libpal
LOCAL_MODULE_OWNER := verizon

include $(BUILD_SHARED_LIBRARY)