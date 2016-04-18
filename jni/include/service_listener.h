/*
 * Copyright (C) 2013 The Android Open Source Project
 * Copyright (C) 2016 Verizon
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef IOMADM_SERVICE_LISTENER_H
#define IOMADM_SERVICE_LISTENER_H

#include <utils/RefBase.h>
#include <binder/IInterface.h>
#include <binder/Parcel.h>

using namespace android;

namespace omadm_service_api {

// .. is the interface through which the .. notifies the
class OmadmServiceListener : public virtual RefBase
{
public:
    OmadmServiceListener() {}
    virtual ~OmadmServiceListener() {}

    // onRequest is called from ... to
    // notify the .... that a n....
    virtual void onRequest(int32_t Id, String16 message) = 0; // Asynchronous
};

class IOmadmServiceListener : public OmadmServiceListener, public IInterface
{
public:
     DECLARE_META_INTERFACE(OmadmServiceListener);
 };

// ----------------------------------------------------------------------------

class BnOmadmServiceListener : public BnInterface<IOmadmServiceListener>
{
public:
    virtual status_t onTransact( uint32_t code, const Parcel& data, Parcel* reply,
            uint32_t flags = 0);
};

class OmadmListener: public BnOmadmServiceListener
{
public:
    static sp<IOmadmServiceListener> mRemoteLst;
    virtual void onRequest(int32_t Id, String16 message);
};

}; // namespace omadm_service_api

#endif