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

#pragma once

#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#include <windows.h>
#include <DirectXMath.h>
#include <TraceLoggingActivity.h>
#include <TraceLoggingProvider.h>

#include <atomic>
#include <memory>
#include <thread>

#include <openvr_driver.h>
#include <driverlog.h>

#include <detours.h>

#define ENABLE_PSVR2_EYE_TRACKING 1
#define ENABLE_DUMMY_ET_GAZES 0
#define ENABLE_PIMAX_EYE_TRACKING (!ENABLE_PSVR2_EYE_TRACKING && 1)

#if ENABLE_PIMAX_EYE_TRACKING
#include <PVR.h>
#include <PVR_API.h>
#include <PVR_Interface.h>
#endif