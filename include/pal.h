#ifndef PAL_H_
#define PAL_H_

#define NO_ERROR                0
#define ERROR_ARGS              1
#define ERROR_SYSCALL           2
#define ERROR_RETURN            3
#define ERROR_INIT              4

#define MAX_DATA_SIZE           127

#ifdef __cplusplus
extern "C" {
#endif


typedef struct
{
    unsigned int data_size;
    char *data_buffer;
} data_buffer_t;


/**
 * This method gets Device ID (IMEI)
 * @param pointer to allocated buffer for Device ID
 * @return execution status: 0 on success or error code
 * on failure.
 */
int pal_system_hostdevice_deviceID_get(data_buffer_t *Buffer);

/**
 * This method sends user notification
 * @param buffer with title of message
 * @param buffer with message body
 * @return execution status 0 on success or error code
 * on failure.
 */
int pal_device_volumes_notifications_set(data_buffer_t *title, data_buffer_t *message);

/**
 * This method gets Device ID (IMEI)
 * @param pointer to allocated buffer for Device ID
 * @return execution status: 0 on success or error code
 * on failure.
 */
int pal_device_wifi_enable(int enable);

/**
 * This method initializes PAL
 * @return execution status: 0 on success or error code
 * on failure.
 * @note Sets BpOmadmServiceAPI::mInitialized to true
 * @note Thist method can block forever if PAL service
 * not found.
 */
int pal_init();

#ifdef __cplusplus
}
#endif // extern C
#endif // PAL_H_
