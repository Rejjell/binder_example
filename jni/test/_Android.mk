# C++11 support is enabled
LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES:= pal_test.cpp

LOCAL_ALLOW_UNDEFINED_SYMBOLS := true
LOCAL_LDLIBS += -L$(LOCAL_PATH)/../lib/support_lib2
LOCAL_SHARED_LIBRARIES := libpal libc++ libutils libbinder liblog libcutils

OMADM_C_INCLUDES := $(LOCAL_PATH)/../include \
                    $(LOCAL_PATH)/../../../../frameworks/base/include/ \
                    $(LOCAL_PATH)/../../../../system/core/include/ \
                    $(LOCAL_PATH)/../../../../external/libcxx/include/ \

LOCAL_C_INCLUDES := $(LOCAL_PATH) $(LOCAL_PATH)/../include
LOCAL_C_INCLUDES += $(OMADM_C_INCLUDES)

LOCAL_CLANG := true
LOCAL_CPPFLAGS := \
        -std=c++11 \

LOCAL_MODULE_TAGS := optional

LOCAL_MODULE:= paltest
LOCAL_MODULE_OWNER := verizon

include $(BUILD_EXECUTABLE)
