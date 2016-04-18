#include <private/android_filesystem_config.h>
#include <sys/capability.h>
#include <sys/prctl.h>
#include <utils/Mutex.h>

#include "omadm_service_api.h"
#include "pal.h"

#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "PAL"

using namespace android;
using namespace omadm_service_api;

// Shared mutex object
static Mutex gPalLock;

// Initialize static fields
sp<IOmadmServiceAPI> BpOmadmServiceAPI::mBindrToService = NULL;
sp<DeathNotifier> DeathNotifier::mDeathNotifier = NULL;
bool BpOmadmServiceAPI::mInitialized = false;
sp<IOmadmServiceListener> OmadmListener::mRemoteLst = NULL;

    /*
     * Helpers declaration
     */
bool isPalInitialized();
int init_binder(bool wait_forever);


    /*******************************************************************************
     *
     * User accessible interface
     */

    /**
     * This method initializes PAL and creates binder to
     * omadm service in framework.
     * @return execution status: 0 on success or error code
     * on failure.
     * @note Sets BpOmadmServiceAPI::mInitialized to true
     * @note Thist method blocks forever if service
     * not found.
     */
int pal_init() {
    AutoMutex _l(gPalLock);
    return init_binder(true);
}

    /**
     * This method gets Device ID (IMEI)
     * @param pointer to allocated buffer for Device ID
     * @return execution status: 0 on success or error code
     * on failure.
     */
int pal_system_hostdevice_deviceID_get(data_buffer_t *Buffer) {
    AutoMutex _l(gPalLock);

    if(!isPalInitialized())
        return ERROR_INIT;

    if((NULL == Buffer)||(NULL == Buffer->data_buffer)||
            (0 == Buffer->data_size))
        return ERROR_ARGS;

    String16* reply = NULL;
    reply = BpOmadmServiceAPI::mBindrToService->getDeviceId();

    // Can return NULL on failure
    if(NULL == reply)
        return ERROR_RETURN;

    if((reply->size() + 1) > Buffer->data_size) {
        delete reply;
        return ERROR_ARGS;
    }
    memset(Buffer->data_buffer, 0, Buffer->data_size);
    strncpy(Buffer->data_buffer, String8(*reply).string(), Buffer->data_size);
    delete reply;
#ifdef DEBUG
    ALOGD("Device ID %s", Buffer->data_buffer);
#endif
    return NO_ERROR;
}

    /**
     * This method sets WiFi On or Off
     * @param bool enable or disable
     * @return execution status: 0 on success or error code
     * on failure.
     */
int pal_device_wifi_enable(int enable) {
    AutoMutex _l(gPalLock);

    if(!isPalInitialized())
        return ERROR_INIT;

    // returns true on success
    bool reply = BpOmadmServiceAPI::mBindrToService->setWifiEnabled(enable);
#ifdef DEBUG
    ALOGD("Set Wifi Enabled %d; return: %d",enable, reply);
#endif
    return reply ? NO_ERROR : ERROR_RETURN;
}

    /**
     * This method sends user notification
     * @param buffer with title of message
     * @param buffer with message body
     * @return execution status 0 on success or error code
     * on failure.
     */
int pal_device_volumes_notifications_set(data_buffer_t *title, data_buffer_t *message) {
    AutoMutex _l(gPalLock);

    if(!isPalInitialized())
        return ERROR_INIT;

    if((NULL == title)||(NULL == title->data_buffer)||
            (0 == title->data_size)||(NULL == message)||
            (NULL == message->data_buffer)||(0 == message->data_size))
        return ERROR_ARGS;
    String16 ts(title->data_buffer);
    String16 ms(message->data_buffer);
    bool reply = BpOmadmServiceAPI::mBindrToService->sendNotification(&ts, &ms);
#ifdef DEBUG
    ALOGD("Send Notification return: %d", reply);
#endif
    return reply ? NO_ERROR : ERROR_RETURN;
}


    /**
     * This method adds listener to the service
     * @param local callback
     * @return execution status 0 on success or error code
     * on failure.
     */
/*int pal_register_listener(userCallback local_callback) {
    AutoMutex _l(gPalLock);

    if(!isPalInitialized())
        return ERROR_INIT;

    if (OmadmListener::mRemoteLst == NULL) {
        OmadmListener::mRemoteLst = new OmadmListener();
    }
    int reply = BpOmadmServiceAPI::mBindrToService->addListener(OmadmListener::mRemoteLst, local_callback);
#ifdef DEBUG
    ALOGD("pal_register_listener return: %d", reply);
#endif
    return reply ? NO_ERROR : ERROR_RETURN;
}
*/
    /**
     * This method removes listener from the service
     * @return execution status 0 on success or error code
     * on failure.
     */
/*int pal_unregister_listener() {
    AutoMutex _l(gPalLock);

    if(!isPalInitialized())
        return ERROR_INIT;

    if (OmadmListener::mRemoteLst == NULL)
        return ERROR_ARGS;

    int reply = BpOmadmServiceAPI::mBindrToService->removeListener(OmadmListener::mRemoteLst);
#ifdef DEBUG
    ALOGD("pal_unregister_listener return: %d", reply);
#endif
    return reply ? NO_ERROR : ERROR_RETURN;
}
*/    /*
     * End of user accessible interface
     ***********************************************************************************/

    /*
     * Helpers
     */

    // Internal method unsynchronized
int init_binder(bool wait_forever) {
    sp<IBinder> binder;
    //get service manager
    sp<IServiceManager> sm = defaultServiceManager();
    do{
        //Search service by SERVICE_NAME
        binder = sm->getService(String16(SERVICE_NAME));
        if((binder != NULL)||(!wait_forever))
            break;
        ALOGE("Service %s is not found. Waiting...", SERVICE_NAME);
        sleep(1);
    }while(1);

    if(binder == NULL)
        return ERROR_SYSCALL;

    if (DeathNotifier::mDeathNotifier == NULL) {
        DeathNotifier::mDeathNotifier = new DeathNotifier();
    }
    binder->linkToDeath(DeathNotifier::mDeathNotifier);

    BpOmadmServiceAPI::mBindrToService = interface_cast<IOmadmServiceAPI>(binder);
    if(BpOmadmServiceAPI::mBindrToService == NULL) {
        ALOGE("Interface cast failed %s", SERVICE_NAME);
        return ERROR_SYSCALL;
    }

    // Start a Binder thread pool to receive Death notification callbacks
    sp<ProcessState> proc(ProcessState::self());
    ProcessState::self()->startThreadPool();

    ALOGI("PAL has started");
    BpOmadmServiceAPI::mInitialized = true;

    return NO_ERROR;
}

bool isPalInitialized() {
    if(BpOmadmServiceAPI::mInitialized) {
        return true;
    }
    else {
        ALOGW("PAL is not initialized");
        int reply = init_binder(true);
        return (NO_ERROR == reply) ? true : false;
    }
}