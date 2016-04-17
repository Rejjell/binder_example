#include <private/android_filesystem_config.h>
#include <sys/capability.h>
#include <sys/prctl.h>

#include "omadm_service_api.h"


#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "OmadmServiceAPI"

using namespace android;

    /*
     * PAL proxy to omadm_service in java framework.
     * Keep in sync with android.omadm.IOmadmManager.
     *
     */

namespace omadm_service_api {

    BpOmadmServiceAPI::BpOmadmServiceAPI(const sp<IBinder>& impl ):BpInterface<IOmadmServiceAPI>(impl) {}

    void DeathNotifier::binderDied(const wp<IBinder>& who) {
        ALOGW("Omadm Service: Binder Died");
        BpOmadmServiceAPI::mInitialized = false;
    }


    /**
     * This method returns device IMEI as String16
     * @return string with IMEI on sucess or NULL
     * on failure.
     * @note Returns NULL on failure. Please check.
     * @note Allocates String16 object it is caller
     * responsibility to free it.
     */
    String16* BpOmadmServiceAPI::getDeviceId()
    {
        Parcel data, reply;

        data.writeInterfaceToken(IOmadmServiceAPI::getInterfaceDescriptor());
        //By operation code to transact
        remote()->transact(GET_DEVICE_ID, data, &reply);
        //Exception Code. In Java Level, aidl auto generate codes will process exceptioncode.
        int32_t err = reply.readExceptionCode();
        if (err < 0) {
            ALOGE("getDeviceId() caught exception %d", err);
            return NULL;
        }
        String16* s = new String16(reply.readString16());;
        return s;
    }

    /**
     * This method turns WiFi On or Off
     * @param boolean: enable or disable WiFi
     * @return status of execution: true on success
     */
    bool BpOmadmServiceAPI::setWifiEnabled(bool enable)
    {
        Parcel data, reply;
        data.writeInterfaceToken(IOmadmServiceAPI::getInterfaceDescriptor());
        data.writeInt32(enable);
        remote()->transact(SET_WIFI_ENABLED, data, &reply);
        int32_t err = reply.readExceptionCode();
        if (err < 0) {
            ALOGE("setWifiEnabled() caught exception %d", err);
            return false;
        }
        return (bool)reply.readInt32();
    }

    /**
     * This method sends user notification
     * @param string: notification title
     * @param string: notification body
     * @return status of execution: true on success
     */
    bool BpOmadmServiceAPI::sendNotification(String16* title, String16* message)
    {
        Parcel data, reply;
        data.writeInterfaceToken(IOmadmServiceAPI::getInterfaceDescriptor());
        data.writeString16(*title);
        data.writeString16(*message);
        remote()->transact(SEND_NOTIFICATION, data, &reply);
        int32_t err = reply.readExceptionCode();
        if (err < 0) {
            ALOGE("sendNotification() caught exception %d", err);
            return false;
        }
        return (bool)reply.readInt32();
    }

    /*
     * Add your methods here
     */

    IMPLEMENT_META_INTERFACE(OmadmServiceAPI,META_INTERFACE_NAME);

}//end of namespace omadm_service_api
