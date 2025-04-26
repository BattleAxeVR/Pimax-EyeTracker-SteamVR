//--------------------------------------------------------------------------------------
// Copyright (c) 2025 BattleAxeVR. All rights reserved.
//--------------------------------------------------------------------------------------

// Author: Bela Kampis
// Date: April 11, 2025

// MIT License
//
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this softwareand associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
//
// The above copyright noticeand this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef PSVR2_EYE_TRACKING_H
#define PSVR2_EYE_TRACKING_H

#include "pch.h"

#if ENABLE_PSVR2_EYE_TRACKING

#include "ipc_client.h"

namespace BVR 
{
    class PSVR2EyeTracker 
    {
      public:
        PSVR2EyeTracker();

        bool connect();
        void disconnect();
        bool are_gazes_available() const;
        bool get_combined_gaze(DirectX::XMVECTOR& gaze_vector);
        bool get_per_eye_gaze(const int eye, DirectX::XMVECTOR& gaze_vector);

      private:
        IPCClient ipc_client_;
        bool is_connected_ = false;
    };

}


#endif // ENABLE_PSVR2_EYE_TRACKING


#endif // PSVR2_EYE_TRACKING_H


