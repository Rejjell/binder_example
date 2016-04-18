/*
 * Copyright 2013, The Android Open Source Project
 * Copyright 2016, Verizon
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdint.h>
#include <sys/types.h>

#include <binder/Parcel.h>
#include <binder/IPCThreadState.h>
#include <binder/IServiceManager.h>

#include "service_listener.h"

using namespace android;

namespace omadm_service_api {

/*
 * API serial code.
 * Keep in sync with android.omadm.IOmadmServiceListener.aidl
 *
 */
enum {
    ON_REQUEST = IBinder::FIRST_CALL_TRANSACTION,
    // Add your codes here
};

// ----------------------------------------------------------------------------

class BpOmadmServiceListener: public BpInterface<IOmadmServiceListener>
{

public:
    BpOmadmServiceListener(const sp<IBinder>& impl)
        : BpInterface<IOmadmServiceListener>(impl) {}

    virtual void onRequest(int32_t Id, String16 message)
    {
        Parcel data, reply;
        data.writeInterfaceToken(IOmadmServiceListener::getInterfaceDescriptor());
        data.writeInt32(Id);
        data.writeString16(message);
        remote()->transact(ON_REQUEST, data, &reply, IBinder::FLAG_ONEWAY);
        reply.readExceptionCode();
    }
};

IMPLEMENT_META_INTERFACE(OmadmServiceListener,
                         "android.omadm.IOmadmServiceListener");

// ----------------------------------------------------------------------

status_t BnOmadmServiceListener::onTransact(
    uint32_t code, const Parcel& data, Parcel* reply, uint32_t flags)
{
    switch(code) {
        case ON_REQUEST: {
            CHECK_INTERFACE(IOmadmServiceListener, data, reply);

            int32_t Id = data.readInt32();
            String16 message = data.readString16();
            // One way interface no reply
            onRequest(Id, message);
            reply->writeNoException();

            return NO_ERROR;
        } break;
        default:
            return BBinder::onTransact(code, data, reply, flags);
    }
}

}; // namespace omadm_service_api