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

#include "pch.h"

#if ENABLE_PSVR2_EYE_TRACKING

// Temp hack until the code upstream is published, to keep this file private / offline
#include "ipc_client.cpp"
#include "psvr2_gazes.h"
#include "psvr2_eye_tracking.h"

namespace BVR 
{
PSVR2EyeTracker::PSVR2EyeTracker()
{
}


bool PSVR2EyeTracker::connect()
{
    if (!is_connected_ && ipc_client_.Connect()) 
    {
        is_connected_ = true;
        return true;
    }

    return false;
}

void PSVR2EyeTracker::disconnect()
{
    // No need or even support for this yet
    if (is_connected_) 
    {
        //is_connected_ = false;
    }
}

bool PSVR2EyeTracker::are_gazes_available() const 
{
#if 0
    if (!is_connected_)
    {
        return false;
    }

    ipc::Request_t request_gaze_packet(ipc::RequestHandshake);
    ipc::Response_t response = ipc_client_.SendBlocking(request_gaze_packet);

    if (response.type == ipc::ResponseHandshake) 
    {
        return true;
    }

    return false;
#else
    return true;
#endif
}

bool PSVR2EyeTracker::get_combined_gaze(DirectX::XMVECTOR& gaze_vector) 
{
    connect();

    if (!is_connected_) 
    {
        return false;
    }

    ipc::Request_t request_gaze_packet(ipc::RequestUpdateGazeState);
    ipc::Response_t response = ipc_client_.SendBlocking(request_gaze_packet);

    if (response.type == ipc::ResponseSuccess) 
    {
        Psvr2GazeState_t* pLocalGazeState = (Psvr2GazeState_t*)&response.gazeState;

        if (pLocalGazeState) 
        {
            const GazeCombined_t& combined_gaze = pLocalGazeState->packetData.combined;

            if (combined_gaze.bIsValid && combined_gaze.bNormalisedGazeValid)
            {
                gaze_vector = DirectX::XMVectorSet(combined_gaze.vNormalisedGaze.x, combined_gaze.vNormalisedGaze.x, combined_gaze.vNormalisedGaze.x, 1);
				return true;
            }
            
        }
    }

    return false;
}

bool PSVR2EyeTracker::get_per_eye_gaze(const int eye, DirectX::XMVECTOR& gaze_vector) 
{
    connect();

    if (!is_connected_) 
    {
        return false;
    }

    ipc::Request_t request_gaze_packet(ipc::RequestUpdateGazeState);
    ipc::Response_t response = ipc_client_.SendBlocking(request_gaze_packet);

    if (response.type == ipc::ResponseSuccess) 
    {
        Psvr2GazeState_t* pLocalGazeState = (Psvr2GazeState_t*)&response.gazeState;

        if (pLocalGazeState) 
        {
            const EyeGaze_t& per_eye_gaze = (eye == 0) ? pLocalGazeState->packetData.left : pLocalGazeState->packetData.right;

            if (per_eye_gaze.bGazeDirectionValid)// !per_eye_gaze.blink)
            {
				gaze_vector = DirectX::XMVectorSet(per_eye_gaze.vGazeDirection.x, per_eye_gaze.vGazeDirection.x, per_eye_gaze.vGazeDirection.x, 1);
				return true;
            }
        }
    }

    return false;
}

} // BVH


#endif // ENABLE_PSVR2_EYE_TRACKING