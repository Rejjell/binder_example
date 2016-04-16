#ifndef __OMADM_SERVICE_API__
#define __OMADM_SERVICE_API__ 1

#include <binder/IBinder.h>
#include <binder/IInterface.h>
#include <binder/IPCThreadState.h>
#include <binder/IServiceManager.h>
#include <binder/Parcel.h>
#include <binder/ProcessState.h>
#include <utils/Log.h>
#include <utils/String16.h>



using namespace android;

#define SERVICE_NAME "omadm_service"
#define META_INTERFACE_NAME "android.omadm.IOmadmManager"

namespace omadm_service_api {

    /*
     * API serial code.
     * Keep in sync with android.omadm.IOmadmManager.aidl
     *
     */
    enum OMADM_API_ENUM{
        GET_DEVICE_ID = IBinder::FIRST_CALL_TRANSACTION,
        SET_WIFI_ENABLED,
        SEND_NOTIFICATION
        // Add your serial code here
    };

    /*
     * Binder Shared interface, stub code
     */
    class IOmadmServiceAPI : public IInterface
    {
        public:
            DECLARE_META_INTERFACE(OmadmServiceAPI);

            //Service API
            virtual String16* getDeviceId() = 0;
            virtual bool setWifiEnabled(bool enable) = 0;
            virtual bool sendNotification(String16* title, String16* message) = 0;
            // Add your methods here
    };

    /*
     * The proxy between the PAL and Service
     */
    class BpOmadmServiceAPI : public BpInterface<IOmadmServiceAPI>
    {
        public:
            static bool mInitialized;
            static sp<IOmadmServiceAPI> mBindrToService;

            BpOmadmServiceAPI(const sp<IBinder>& impl );

            // Service API
            virtual String16* getDeviceId();
            virtual bool setWifiEnabled(bool enable);
            virtual bool sendNotification(String16* title, String16* message);
            // Add your methods here
    };


    class DeathNotifier: public IBinder::DeathRecipient
    {
    public:
        static sp<DeathNotifier> mDeathNotifier;

        DeathNotifier() {
        }
        virtual void binderDied(const wp<IBinder>& who);
    };
} //end of namespace omadm_service_api
#endif //end of ifndef __OMADM_SERVICE_API__
