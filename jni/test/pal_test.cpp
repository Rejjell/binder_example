#include <condition_variable>
#include <mutex>
#include <private/android_filesystem_config.h>
#include <stdio.h>
#include <string.h>
#include <thread>
#include <utils/Log.h>
//#include <utils/Vector.h>

#include "pal.h"


#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "PAL_TEST"

using namespace android;

static std::mutex mtx;
static std::condition_variable cv;
//static Vector<request_t> input_events_queue;

    /*********************************************************************
     * Helpers
     */

     // callback function
void pushUserEvent(request_t event) {
    std::unique_lock<std::mutex> lck(mtx);
    //input_events_queue.push_front(event);
    cv.notify_one();
    ALOGD("pushUserEvent(): tid: %d", gettid());
}
    // wait-notify helper
bool is_events_available() {
    return false; //!input_events_queue.isEmpty();
}

    // Allocates PAL data buffer
void getNewDataBuffer(data_buffer_t* pDBuffer, unsigned int size) {
    pDBuffer->data_buffer = new char[size];
    pDBuffer->data_size = size;
    memset(pDBuffer->data_buffer, 0, pDBuffer->data_size);
}

    // Releases PAL data buffer
void freeDataBuffer(data_buffer_t* pDBuffer) {
    if((NULL != pDBuffer->data_buffer) && (0 != pDBuffer->data_size)) {
        delete[] pDBuffer->data_buffer;
        pDBuffer->data_buffer = NULL;
        pDBuffer->data_size = 0;
    }
}

    // Clears content of PAL data bufer
void clearDataBuffer(data_buffer_t* pDBuffer) {
    if((NULL != pDBuffer->data_buffer) && (0 != pDBuffer->data_size)) {
        memset(pDBuffer->data_buffer, 0, pDBuffer->data_size);
    }
}

    // Wraps given string to PAL data buffer
void wrapDataBuffer(char* strToWrap, data_buffer_t* pDBuffer) {
    int size = strlen(strToWrap);
    pDBuffer->data_buffer = new char[size + 1];
    pDBuffer->data_size = size + 1;
    memset(pDBuffer->data_buffer, 0, pDBuffer->data_size);
    strncpy(pDBuffer->data_buffer, strToWrap, size);
}
    /**********************************************************************/

    /*
     * Main Test routine
     */
int main(int argc, char *argv[])
{
    UNUSED(argc);
    UNUSED(argv);
    int reply = 0;
    ALOGI("I: Tests runed");

    data_buffer_t dataBuffer;

        // Initialize PAL. Get binder to framework service.
        if ( NO_ERROR != pal_init()) {
            ALOGE("Could not initialize PAL");
            return 1;
        }

        /******************************************************************
         * Start to check PAL API
         *
         */

        /*
         * Get Device ID
         */

        // Get Device ID (normal case)
        getNewDataBuffer(&dataBuffer, MAX_DATA_SIZE);
        reply = pal_system_hostdevice_deviceID_get(&dataBuffer);
        if(NO_ERROR != reply) {
            ALOGE("pal_system_hostdevice_deviceID_get() <normal case> error: %d", reply);
        } else {
            ALOGD("pal_system_hostdevice_deviceID_get() <normal case> : %s",
                    dataBuffer.data_buffer);
        }
        freeDataBuffer(&dataBuffer);
        reply = 0;

        // Get Device ID (NULL buffer)
        reply = pal_system_hostdevice_deviceID_get(&dataBuffer);
        if(NO_ERROR != reply) {
            ALOGE("pal_system_hostdevice_deviceID_get() <NULL buffer> error: %d", reply);
        } else {
            ALOGD("pal_system_hostdevice_deviceID_get() <NULL buffer> : %s",
                    dataBuffer.data_buffer);
        }
        reply = 0;

        // Get Device ID (small buffer)
        getNewDataBuffer(&dataBuffer, 2);
        reply = pal_system_hostdevice_deviceID_get(&dataBuffer);
        if(NO_ERROR != reply) {
            ALOGE("pal_system_hostdevice_deviceID_get() <small buffer> error: %d", reply);
        } else {
            ALOGD("pal_system_hostdevice_deviceID_get() <small buffer> : %s",
                    dataBuffer.data_buffer);
        }
        freeDataBuffer(&dataBuffer);
        reply = 0;

        /*
         * Set WiFi enabled / disabled
         */

        // Set WiFi enabled
        reply = pal_device_wifi_enable(1);
        if(NO_ERROR != reply) {
            ALOGE("pal_device_wifi_enable(1)error: %d", reply);
        } else {
            ALOGD("pal_device_wifi_enable(1): %d", reply);
        }
        reply = 0;

        sleep(2);

        // Set WiFi disabled
        reply = pal_device_wifi_enable(0);
        if(NO_ERROR != reply) {
            ALOGE("pal_device_wifi_enable(0) error: %d", reply);
        } else {
            ALOGD("pal_device_wifi_enable(0): %d", reply);
        }
        reply = 0;

        /*
         * Send user notification
         */

        data_buffer_t titleBuffer;
        data_buffer_t messageBuffer;
        char title[] = "New updates available";
        char message[] = "Start download";
        wrapDataBuffer(title, &titleBuffer);
        wrapDataBuffer(message, &messageBuffer);

        reply = pal_device_volumes_notifications_set(&titleBuffer, &messageBuffer);
        if(NO_ERROR != reply) {
            ALOGE("pal_device_volumes_notifications_set()) error: %d", reply);
        }
        reply = 0;
        freeDataBuffer(&titleBuffer);
        freeDataBuffer(&messageBuffer);


        /*
         * Test Death recipient and Callbacks
         * Emulation of the main loop of the DM Client.
         * Lopos for some time.
         */

        /*
         * Add Listener
         */
        /*userCallback callback = pushUserEvent;

        reply = pal_register_listener(callback);
        if(NO_ERROR != reply) {
            ALOGE("pal_register_listener() error: %d", reply);
        }
        reply = 0;

        // Start main loop for a while...
        for(int i=0; i<10; i++) {
            while(1) {
                {
                    std::unique_lock<std::mutex> lck(mtx);
                    if(!is_events_available()) break;
                    ALOGI("Extracting event: < %s; %d >; queue size: %d",
                            input_events_queue.top().message,
                            input_events_queue.top().Id,
                            input_events_queue.size());

                    delete[] input_events_queue.top().message;
                    input_events_queue.pop();
                }
                ALOGI("main loop is processing event... tid: %d", gettid());
                sleep(1);
            }
            // Wait for new user event
            ALOGI("** Waiting for new event(s)... **");
            std::unique_lock<std::mutex> lck(mtx);
            cv.wait(lck,is_events_available);
        }
        */
         /*
         * Remove Listener
         */
        /*reply = pal_unregister_listener();
        if(NO_ERROR != reply) {
            ALOGE("pal_unregister_listener() error: %d", reply);
        }
        reply = 0;
        */
        /******************************************************************/

        /*
         * Add your tests here
         */


        /*
         * End to check PAL API
         ******************************************************************
         */

    return 0;
}

