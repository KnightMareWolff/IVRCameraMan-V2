#ifndef CIVRVIRTUALCAMERA_H
#define CIVRVIRTUALCAMERA_H

#include "IVRLowLevelSDK_global.h"
#include "IVR_Frame.h"
#include "IVR_CameraTake.h"
#include "IVR_LockFreeQueue.h"
#include "IVR_ImageHandler.h"
#include "IVR_Shrinker.h"
#include "IVR_IFFReader.h"
#include "IVR_MediaHandler.h"

#define QUEUE_SIZE 2000;

class IVRLOWLEVELSDK_EXPORT CIVRVirtualCamera : public QObject
{
public:
    CIVRVirtualCamera();
   ~CIVRVirtualCamera();

    CIVRVirtualCamera( const CIVRVirtualCamera & copy )
    {
        IVR_CameraName      = copy.IVR_CameraName;
        IVR_RootFolder      = copy.IVR_RootFolder;
        IVR_CameraType      = copy.IVR_CameraType;
        IVR_DesiredFPS      = copy.IVR_DesiredFPS;
        IVR_RecordingMode   = copy.IVR_RecordingMode;
        IVR_IsStabilized    = copy.IVR_IsStabilized;
        IVR_IsEnabled       = copy.IVR_IsEnabled;
        IVR_IsRecording     = copy.IVR_IsRecording;
        IVR_haveOpenTake    = copy.IVR_haveOpenTake;
        IVR_Takes           = copy.IVR_Takes;
        IVR_ActualTake      = copy.IVR_ActualTake;
        IVR_Timestamp       = copy.IVR_Timestamp;
        IVR_FrameCounter    = copy.IVR_FrameCounter;
        IVR_EffectsEnabled  = copy.IVR_EffectsEnabled;
        IVR_EffectsName     = copy.IVR_EffectsName;
    }

    CIVRVirtualCamera& operator = (const CIVRVirtualCamera &t)
    {
        IVR_CameraName      = t.IVR_CameraName;
        IVR_RootFolder      = t.IVR_RootFolder;
        IVR_CameraType      = t.IVR_CameraType;
        IVR_DesiredFPS      = t.IVR_DesiredFPS;
        IVR_RecordingMode   = t.IVR_RecordingMode;
        IVR_IsStabilized    = t.IVR_IsStabilized;
        IVR_IsEnabled       = t.IVR_IsEnabled;
        IVR_IsRecording     = t.IVR_IsRecording;
        IVR_haveOpenTake    = t.IVR_haveOpenTake;
        IVR_Takes           = t.IVR_Takes;
        IVR_ActualTake      = t.IVR_ActualTake;
        IVR_Timestamp       = t.IVR_Timestamp;
        IVR_FrameCounter    = t.IVR_FrameCounter;
        IVR_EffectsEnabled  = t.IVR_EffectsEnabled;
        IVR_EffectsName     = t.IVR_EffectsName;
        return *this;
    }

    bool IVR_RecordBuffer (const IVR_RenderBuffer &pIVR_FrameBuffer, const IVR_FrameData &pIVR_FrameData);

    bool IVR_StartRecord();
    bool IVR_StopRecord ();
    bool IVR_CompileTake();

    //Store the real camera name
    QString                   IVR_CameraName ;
    //Store the place where camera record videos(duplicated here for optimization reasons)
    QString                   IVR_RootFolder ;
    //Store the real camera type
    uint                      IVR_CameraType ;
    //Recording Elapsed Time
    qint64                    IVR_ElapsedTime;
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
    //Flag to Know if the camera are recording the final video take(.mp4)
    bool                      IVR_IsCompiling;
    //Flag to Know if the camera finish all Its operations(Will be uded by midlevel to delete the camera)
    bool                      IVR_IsIddle;
    //Flag to Know if the camera have an Open Take to record Buffers
    bool                      IVR_haveOpenTake;
    //For every video recorded in a session, we record it in a take and cleanup the frames.
    vector<CIVRCameraTake*>   IVR_Takes;
    //When recording buffers the actual take will be used to direct access
    uint                      IVR_ActualTake;
    //Timestamp of the Camera Creation
    qint64                    IVR_Timestamp;
    //Timestamp of the Camera Creation
    qint64                    IVR_FrameCounter;
    //Flag to Know if we need apply a Special Effect;
    bool                      IVR_EffectsEnabled;
    //Store the special effects filename
    QString                   IVR_EffectsName;
    //Flag to Know if we need apply a Special Effect;
    bool                      IVR_CompressionEnabled;
    //Counter for the compilations made
    uint                      IVR_Compilations;

    //Thread Used to record Buffers
    QFuture<void> IVR_CPThread;
    QFuture<void> IVR_BFThread;

    //Timer Used to break the Takes in many pieces
    QElapsedTimer IVR_CameraTimer;
    QElapsedTimer IVR_TakeTimer;

};

#endif // CIVRVIRTUALCAMERA_H
