#ifndef IVRLOWLEVELSDK_H
#define IVRLOWLEVELSDK_H

#include "IVRLowLevelSDK_global.h"
#include "IVR_ImageHandler.h"
#include "IVR_ImageFilter.h"
#include "IVR_RealCamera.h"
#include "IVR_VideoReader.h"
#include "IVR_VirtualCamera.h"
#include "IVR_CameraTake.h"
#include "IVR_LockFreeQueue.h"
#include "IVR_MediaHandler.h"
#include "IVR_Classifier.h"
#include "IVR_Shrinker.h"


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
    void IVR_SetResolution(int  pIVR_Width,int  pIVR_Height);
    void IVR_SpawnStage   (                         );

    CIVRVirtualCamera* IVR_AddVirtualCam(uint             &pIVR_CameraId    ,
                                         uint              pIVR_CamType     ,
                                         uint              pIVR_RecMode     ,
                                         QString          &pIVR_CameraName  ,
                                         qint64            pIVR_Timestamp   );

    void IVR_RemoveVirtualCam(uint pIVR_CameraId);

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

    uint IVR_GetCounter(uint pIVR_CamType);

    //Miscellaneous Functions
    bool IVR_CheckRecordingState  (uint pIVR_CameraId );
    bool IVR_CleanUpRecordVSession(                   );

    //Configuration Folders
    QString IVR_RootFolder;
    QString IVR_ConfigFolder;
    //Width of the Image
    int     IVR_Width       ;
    //Height of the Image
    int     IVR_Height      ;

    static vector<CIVRVirtualCamera*> IVR_CameraPool;

    //GUI Thread
    QFuture<void>         IVR_GUIThread;

    QApplication*  IVR_QtApp;
    bool           IVR_GUIActive;
    bool           IVR_PipeActive;

    bool           IVR_DebbugMode;
    uint           IVR_MSecToWait;

    QMutex          mutex;
    QWaitCondition  newdataAvailable;

    //Internal Static Config for LowLevel classes
    CIVRConfig     IVR_Config;
};

#endif // IVRLOWLEVELSDK_H
