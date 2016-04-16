LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES:= pal_test.cpp
LOCAL_SHARED_LIBRARIES := libutils liblog libcutils libpal

OMADM_C_INCLUDES := frameworks/base/include/ \
                    system/core/include/

LOCAL_C_INCLUDES := $(LOCAL_PATH) $(LOCAL_PATH)/../include
LOCAL_C_INCLUDES += $(OMADM_C_INCLUDES)

LOCAL_MODULE_TAGS := optional

LOCAL_MODULE:= paltest
LOCAL_MODULE_OWNER := verizon

include $(BUILD_EXECUTABLE)
