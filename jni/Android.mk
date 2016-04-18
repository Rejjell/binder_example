include $(call all-subdir-makefiles)

# C++11 support is enabled
LOCAL_PATH := $(call my-dir)
libs := ../../../work/binder_example/jni/libs
$(info $(libs))

#LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := binder
LOCAL_SRC_FILES := $(libs)/libbinder.so
include $(PREBUILT_SHARED_LIBRARY)

#LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := utils
LOCAL_SRC_FILES := $(libs)/libutils.so
include $(PREBUILT_SHARED_LIBRARY)

#LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := cutils
LOCAL_SRC_FILES := $(libs)/libcutils.so
include $(PREBUILT_SHARED_LIBRARY)
