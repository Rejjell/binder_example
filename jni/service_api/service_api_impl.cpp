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

    /**
     * Death Notifier implementation. (Callback)
     * Catch Binder Died notification in cases of system server
     * crash. This is unlikely event but we will need to
     * re-initialise PAL in this case.
     * param@ who is died
     */
    void DeathNotifier::binderDied(const wp<IBinder>& who) {
        UNUSED(who);
        ALOGW("Omadm Service: Binder Died");
        BpOmadmServiceAPI::mInitialized = false;
    }

    /**
     * Omadm Service Listener implementation.
     * Now it is an example of some "user action" listener
     * param@ (example) Id of user action
     * param@ (example) message from user
     * <p>
     * @note this method allocates request_data.message buffer
     * calee is responsible for freeing!
     */
    userCallback BpOmadmServiceAPI::mCallback = NULL;

    // Sends notification to PAL about user request
    void OmadmListener::onRequest(int32_t Id, String16 message) {
        String8 m = String8(message);

        request_t request_data;
        unsigned int sz = m.size();

        request_data.message = new char[sz + 1];
        if(request_data.message == NULL)
            return;
        memset(request_data.message, 0, (sz + 1));
        strncpy(request_data.message, m.string(), sz);

        request_data.Id = Id;
        if(BpOmadmServiceAPI::mCallback != NULL) {
            BpOmadmServiceAPI::mCallback(request_data);
        } else{
            delete[] request_data.message;
        }
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

    /**
     * This method registers listener
     * @param remote listener object
     * @param local callback
     * @return status of execution: true on success
     */
    /*int BpOmadmServiceAPI::addListener(const sp<IOmadmServiceListener>& listener,
            userCallback callback)
    {
        mCallback = callback;

        ALOGE("Add OmadmService Listener");
        Parcel data, reply;

        data.writeInterfaceToken(IOmadmServiceAPI::getInterfaceDescriptor());
        data.writeStrongBinder(IInterface::asBinder(listener));
        remote()->transact(ADD_LISTENER, data, &reply);
        int32_t err = reply.readExceptionCode();
        return reply.readInt32();
    }
*/
    /**
     * This method unregisters listener
     * @param remote listener object
     * @return status of execution: true on success
     */
    /*int BpOmadmServiceAPI::removeListener(const sp<IOmadmServiceListener>& listener)
        {
        ALOGE("Remove OmadmService Listener");
        Parcel data, reply;

        data.writeInterfaceToken(IOmadmServiceAPI::getInterfaceDescriptor());
        data.writeStrongBinder(IInterface::asBinder(listener));
        remote()->transact(REMOVE_LISTENER, data, &reply);
        int32_t err = reply.readExceptionCode();
        return reply.readInt32();
    }
    */
    /*
     * Add your methods here
     */

    IMPLEMENT_META_INTERFACE(OmadmServiceAPI,META_INTERFACE_NAME);

}//end of namespace omadm_service_api
