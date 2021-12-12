#ifndef CIVRVIRTUALCAMERA_H
#define CIVRVIRTUALCAMERA_H

#include "IVRLowLevelSDK_global.h"
#include "IVR_Frame.h"
#include "IVR_CameraTake.h"
#include "IVR_TaskQueue.h"

class IVRLOWLEVELSDK_EXPORT CIVRVirtualCamera
{
public:
    CIVRVirtualCamera();

    CIVRVirtualCamera( const CIVRVirtualCamera & copy )
    {
        IVR_CameraName      = copy.IVR_CameraName;
        IVR_CameraType      = copy.IVR_CameraType;
        IVR_DesiredFPS      = copy.IVR_DesiredFPS;
        IVR_RecordingMode   = copy.IVR_RecordingMode;
        IVR_IsStabilized    = copy.IVR_IsStabilized;
        IVR_IsEnabled       = copy.IVR_IsEnabled;
        IVR_IsRecording     = copy.IVR_IsRecording;
        IVR_Frames          = copy.IVR_Frames;
        //IVR_RenderQueue     = copy.IVR_RenderQueue;
        IVR_Takes           = copy.IVR_Takes;
        IVR_Timestamp       = copy.IVR_Timestamp;
    }

    CIVRVirtualCamera& operator = (const CIVRVirtualCamera &t)
    {
        IVR_CameraName      = t.IVR_CameraName;
        IVR_CameraType      = t.IVR_CameraType;
        IVR_DesiredFPS      = t.IVR_DesiredFPS;
        IVR_RecordingMode   = t.IVR_RecordingMode;
        IVR_IsStabilized    = t.IVR_IsStabilized;
        IVR_IsEnabled       = t.IVR_IsEnabled;
        IVR_IsRecording     = t.IVR_IsRecording;
        IVR_Frames          = t.IVR_Frames;
        //IVR_RenderQueue     = copy.IVR_RenderQueue;
        IVR_Takes           = t.IVR_Takes;
        IVR_Timestamp       = t.IVR_Timestamp;
        return *this;
    }

    int  IVR_GetRecFPS();
    int  IVR_GetAverageFPS();
    int  IVR_GetBestFPS();
    int  IVR_GetDesiredFPS();
    int  IVR_GetFPSByTimeApprox();

    bool IVR_CameraTake(QString pIVR_VideoTake);

    //Store the real camera name
    QString                   IVR_CameraName;
    //Store the real camera type
    uint                      IVR_CameraType   ;
    //Store the Last FPS Collected by the camera recordings
    float                     IVR_DesiredFPS;
    //Store the recording mode of the Camera
    uint                      IVR_RecordingMode;
    //If true this camera record Stabilized Frames on the HD.(Used To know the correct files to load).
    bool                      IVR_IsStabilized;
    //Flag to Know the Camera State.(Take Care to not enable it in the wrong time!)
    bool                      IVR_IsEnabled;
    //Flag to Know if the camera are doing low level operations that lock high level cameras
    bool                      IVR_IsRecording;
    //For every frame recorded in a recording take, we record it in a list of frames.
    vector<CIVRFrame>         IVR_Frames;
    //For every Camera we will have a number of tasks to perform...
    CIVRTaskQueue             IVR_RenderQueue;
    //For every video recorded in a session, we record it in a take and cleanup the frames.
    vector<CIVRCameraTake>    IVR_Takes;
    //Timestamp of the Camera Creation
    qint64                    IVR_Timestamp;
};

#endif // CIVRVIRTUALCAMERA_H
