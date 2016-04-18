adb shell mount -o rw,remount,rw /system
rem adb push obj\local\armeabi\libc++_shared.so /system/lib/libc++_shared.so
adb push obj\local\armeabi\libpal.so /system/lib/libpal.so
adb push obj\local\armeabi\paltest /system/bin/paltest
adb shell chmod 555 /system/bin/paltest
doskey pl=adb shell paltest