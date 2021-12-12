#ifndef IVRLOWLEVELSDK_H
#define IVRLOWLEVELSDK_H

#include "IVRLowLevelSDK_global.h"
#include "IVR_ImageReader.h"
#include "IVR_ImageFilter.h"
#include "IVR_CameraReader.h"
#include "IVR_VideoReader.h"
#include "IVR_VirtualCamera.h"
#include "IVR_CameraTake.h"
#include "IVR_TaskQueue.h"


class IVRLOWLEVELSDK_EXPORT IVRLowLevelSDK : public QObject
{
public:
    IVRLowLevelSDK(                       );
    IVRLowLevelSDK(bool bOpenOfflineThread);
   ~IVRLowLevelSDK(                       );

    bool IVR_SetRootPath  (QString &pDAIAbsolutePath);
    bool IVR_SetConfPath  (QString &pDAIAbsolutePath);
    void IVR_SetDebugMode (bool pIVR_DebbugMode     );
    void IVR_SetWaitRecord(uint pIVR_MsecToWait     );
    void IVR_SpawnStage   (                         );

    bool IVR_RecordBuffer (IVR_RenderBuffer &pIVR_FrameBuffer);

    bool IVR_AddVirtualCam(uint             &pIVR_CameraId    ,
                           uint              pIVR_CamType     ,
                           uint              pIVR_RecMode     ,
                           QString          &pIVR_CameraName  ,
                           float             pIVR_FPS         ,
                           bool              pIVR_IsStabilized,
                           bool              pIVR_IsEnabled   ,
                           qint64            pIVR_Timestamp   );

    bool IVR_UpdVirtualCam(uint              pIVR_CameraId    ,
                           uint              pIVR_CamType     ,
                           QString          &pIVR_CameraName  ,
                           float             pIVR_FPS         ,
                           bool              pIVR_IsStabilized,
                           bool              pIVR_IsEnabled   );

    //"Out of the Box" GUI
    void IVR_FireUpGUIThread();
    void IVR_StopGUIThread();
    bool IVR_IsGUIActive();

    //Recording Threads
    void IVR_FireUpThreads();
    void IVR_StopThreads();
    bool IVR_IsThreadsActive();

    //Thread Functions
    void IVR_FireGUIThread();

    //Processing Pipelines
    void IVR_CamRecLoopThread();

    void IVR_RecordTake(uint pIVR_CameraId , QString IVR_RootFolder);

    uint IVR_GetCounter(uint pIVR_CamType);

    //Miscellaneous Functions
    bool IVR_CheckRecordingState  (uint pIVR_CameraId );
    bool IVR_HeadShotVideoRecord  (uint pIVR_CameraId ,uint pIVR_CamType);
    bool IVR_CleanUpRecordVSession(                                     );

    //Configuration Folders
    QString IVR_RootFolder;
    QString IVR_ConfigFolder;

    vector<CIVRVirtualCamera*> IVR_CameraPool;

    //For every Camera we will have a number of tasks to perform...
    CIVRTaskQueue         IVR_RecordingPool;

    //GUI Thread
    QFuture<void>         IVR_GUIThread;

    //Render Threads Used for frames recording
    QFuture<void>         IVR_RCThread;

    QApplication*  IVR_QtApp;
    bool           IVR_GUIActive;
    bool           IVR_PipeActive;

    bool           IVR_DebbugMode;
    uint           IVR_MSecToWait;
};

#endif // IVRLOWLEVELSDK_H
