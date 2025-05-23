// MIT License
//
// Copyright(c) 2025 Matthieu Bucchianeri
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "pch.h"

#include "ShimDriverManager.h"
#include "DetourUtils.h"
#include "Tracing.h"

namespace {
    using namespace driver_shim;

#if ENABLE_PIMAX_EYE_TRACKING
    pvrEnvHandle g_pvr = nullptr;
    pvrSessionHandle g_pvrSession = nullptr;
#endif 

#if ENABLE_PSVR2_EYE_TRACKING
#endif

    DEFINE_DETOUR_FUNCTION(bool,
                           IVRServerDriverHost_TrackedDeviceAdded,
                           vr::IVRServerDriverHost* driverHost,
                           const char* pchDeviceSerialNumber,
                           vr::ETrackedDeviceClass eDeviceClass,
                           vr::ITrackedDeviceServerDriver* pDriver) {
        TraceLocalActivity(local);
        TraceLoggingWriteStart(local,
                               "IVRServerDriverHost_TrackedDeviceAdded",
                               TLArg(pchDeviceSerialNumber, "DeviceSerialNumber"),
                               TLArg((int)eDeviceClass, "DeviceClass"));

        vr::ITrackedDeviceServerDriver* shimmedDriver = pDriver;

        // Only shim the desired device class and if they are registered by the target driver.
        if (IsTargetDriver(_ReturnAddress())) {
            TraceLoggingWriteTagged(local, "IVRServerDriverHost_TrackedDeviceAdded", TLArg(true, "IsTargetDriver"));
            if (eDeviceClass == vr::TrackedDeviceClass_HMD) {
                DriverLog("Shimming new TrackedDeviceClass_HMD with HmdShimDriver");

#if ENABLE_PIMAX_EYE_TRACKING
                shimmedDriver = CreateHmdShimDriver(pDriver, g_pvr, g_pvrSession);
#endif 

#if ENABLE_PSVR2_EYE_TRACKING
                shimmedDriver = CreateHmdShimDriver(pDriver);
#endif 
            }
        }

        const auto status = original_IVRServerDriverHost_TrackedDeviceAdded(
            driverHost, pchDeviceSerialNumber, eDeviceClass, shimmedDriver);

        TraceLoggingWriteStop(local, "IVRServerDriverHost_TrackedDeviceAdded", TLArg(status, "Status"));

        return status;
    }

} // namespace

namespace driver_shim {

#if ENABLE_PIMAX_EYE_TRACKING
    void InstallShimDriverHook(pvrEnvHandle pvr, pvrSessionHandle pvrSession) {
        TraceLocalActivity(local);
        TraceLoggingWriteStart(local, "InstallShimDriverHook");

        DriverLog("Installing IVRServerDriverHost::TrackedDeviceAdded hook");

        g_pvr = pvr;
        g_pvrSession = pvrSession;

        // TODO: Consider hooking all flavors, though I doubt the driver_aapvr will change anytime soon.
        vr::EVRInitError eError;
        DetourMethodAttach(
            // driver_aapvr uses the 006 flavor.
            vr::VRDriverContext()->GetGenericInterface("IVRServerDriverHost_006", &eError),
            0 /* TrackedDeviceAdded() */,
            hooked_IVRServerDriverHost_TrackedDeviceAdded,
            original_IVRServerDriverHost_TrackedDeviceAdded);

        TraceLoggingWriteStop(local, "InstallShimDriverHook");
    }
#endif 

#if ENABLE_PSVR2_EYE_TRACKING
    void InstallShimDriverHook() 
    {
        TraceLocalActivity(local);
        TraceLoggingWriteStart(local, "InstallShimDriverHook");

        DriverLog("Installing IVRServerDriverHost::TrackedDeviceAdded hook");

        // TODO: Consider hooking all flavors, though I doubt the driver_aapvr will change anytime soon.
        vr::EVRInitError eError;
        DetourMethodAttach(
            // driver_aapvr uses the 006 flavor.
            vr::VRDriverContext()->GetGenericInterface("IVRServerDriverHost_006", &eError),
            0 /* TrackedDeviceAdded() */,
            hooked_IVRServerDriverHost_TrackedDeviceAdded,
            original_IVRServerDriverHost_TrackedDeviceAdded);

        TraceLoggingWriteStop(local, "InstallShimDriverHook");
    }
#endif 

    bool IsTargetDriver(void* returnAddress) {
        HMODULE callerModule;
        if (GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                               (LPCSTR)returnAddress,
                               &callerModule)) {

#if ENABLE_PSVR2_EYE_TRACKING
            const bool is_basic_psvr2_dll = (callerModule == GetModuleHandleA("driver_playstation_vr2.dll"));
            const bool is_orig_psvr2_dll = (callerModule == GetModuleHandleA("driver_playstation_vr2_orig.dll"));
            return (is_basic_psvr2_dll || is_orig_psvr2_dll);
#elif ENABLE_PIMAX_EYE_TRACKING
            return callerModule == GetModuleHandleA("driver_aapvr.dll");
#endif
        }
        return false;
    }

} // namespace driver_shim
