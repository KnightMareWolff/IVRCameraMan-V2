#ifndef CIVRCAMERATAKE_H
#define CIVRCAMERATAKE_H

#include "IVRLowLevelSDK_global.h"
#include "IVR_Frame.h"
#include "IVR_LockFreeQueue.h"
#include "IVR_IFFReader.h"
#include "IVR_IIFReader.h"
#include "IVR_ImageHandler.h"
#include "IVR_Shrinker.h"


class IVRLOWLEVELSDK_EXPORT CIVRCameraTake : public QObject
{
public:
    CIVRCameraTake();
   ~CIVRCameraTake();

    CIVRCameraTake( const CIVRCameraTake & copy )
    {
        IVR_TakeID      = copy.IVR_TakeID;
        IVR_TakeState   = 0;
    }

    CIVRCameraTake& operator = (const CIVRCameraTake &t)
    {
        IVR_TakeID      = t.IVR_TakeID;
        IVR_TakeState   = 0;
        return *this;
    }

    //ID to be used with the Name
    uint               IVR_TakeID;
    //ID to be used with the Name
    uint               IVR_CameraID;
    //Store the real camera name
    QString            IVR_CameraName;
    //ID to be used with the Name
    uint               IVR_TakeState; //0-iddle 1-recording 2-compiling 3-Finished
    //Frame counter
    uint               IVR_FrameCounter;
    //Final Take FPS
    uint               IVR_FPS;
    //Take Elapsed Time
    qint64             IVR_ElapsedTime;
    //Store the Desired FPS selected by the User
    float              IVR_DesiredFPS;
    //Store the recording mode Selected by the User
    uint               IVR_RecordingMode;
    //Flag to Know if we need apply a Special Effect;
    bool               IVR_EffectsEnabled;
    //Store the special effects filename
    QString            IVR_EffectsName;
    //Image Handler for Special Effects
    CIVRImageHandler  *IVR_ImageHandler;
    //Flag to Know if we need apply a Special Effect;
    bool               IVR_CompressionEnabled;
    //Final Video filename
    QString            IVR_TakeName;
    //Final Frame filename
    QString            IVR_FrameName;

    uint  IVR_GetRecFPS();

    void IVR_StartRecord ();
    void IVR_StartCompile();
    void IVR_CleanUpTake ();

    void IVR_DataLoop ();
    void IVR_ImageLoop();

    //-----------------------------------------------
    //a Lock Free Queue for FrameBuffer recordings.
    //-----------------------------------------------
    // RenderBuffer size 112 => str * 60 = 6720
    // FrameData    size 48  => str * 60 = 2880
    LockFreeQueue<IVR_RenderBuffer, 6720  > *IVR_RenderQueue;
    LockFreeQueue<IVR_FrameData   , 2880  > *IVR_DataQueue;

    //Thread Used to record Buffers
    QFuture<void> IVR_TakeBFThread; //Thread taking Buffers
    QFuture<void> IVR_TakeCPThread; //Thread compiling the take
    QFuture<void> IVR_TakeDTThread; //Thread taking FrameData

    //Timer Used to calculated The Take Elapsed Time.
    QElapsedTimer IVR_TakeTimer;

};

#endif // CIVRCAMERATAKE_H
