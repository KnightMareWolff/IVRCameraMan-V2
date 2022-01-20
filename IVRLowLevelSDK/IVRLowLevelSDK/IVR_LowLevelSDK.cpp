#include "IVR_LowLevelSDK.h"
#include "IVR_LauncherUI.h"

CIVRLauncherUI   IVR_UILauncher;
vector<CIVRVirtualCamera*> IVRLowLevelSDK::IVR_CameraPool;

IVRLowLevelSDK::IVRLowLevelSDK()
{
   IVR_DebbugMode  = false;
}

IVRLowLevelSDK::IVRLowLevelSDK(bool bOpenOfflineThread)
{
   Q_UNUSED(bOpenOfflineThread);
   IVR_DebbugMode  = false;
}

IVRLowLevelSDK::~IVRLowLevelSDK()
{

}

bool IVRLowLevelSDK::IVR_SetRootPath  (QString &pDAIAbsolutePath)
{
    IVR_RootFolder = QString(pDAIAbsolutePath);
    CIVRConfig::IVR_RootFolder =  QString(pDAIAbsolutePath);

    if(IVR_RootFolder.isEmpty())return false;

    return true;
}

bool IVRLowLevelSDK::IVR_SetConfPath  (QString &pDAIAbsolutePath)
{
    IVR_ConfigFolder = QString(pDAIAbsolutePath);

    CIVRConfig::IVR_ConfigFolder = QString(pDAIAbsolutePath);

    if(IVR_ConfigFolder.isEmpty())return false;

    return true;
}

void IVRLowLevelSDK::IVR_SetDebugMode(bool pIVR_DebbugMode)
{
    IVR_DebbugMode = pIVR_DebbugMode;
    CIVRConfig::IVR_DebbugMode = pIVR_DebbugMode;
}

void IVRLowLevelSDK::IVR_SetWaitRecord(uint pIVR_MsecToWait     )
{
    IVR_MSecToWait = pIVR_MsecToWait;
    CIVRConfig::IVR_MSecToWait = pIVR_MsecToWait;
}

void IVRLowLevelSDK::IVR_SetResolution(int  pIVR_Width,int  pIVR_Height)
{
    IVR_Width              = pIVR_Width;
    IVR_Height             = pIVR_Height;
    CIVRConfig::IVR_Width  = pIVR_Width;
    CIVRConfig::IVR_Height = pIVR_Height;
}

void IVRLowLevelSDK::IVR_SpawnStage()
{
    IVR_QtApp->postEvent( &IVR_UILauncher, new QEvent( QEvent::User ) );
    if(IVR_UILauncher.hasValidUI())
    {
       IVR_UILauncher.attachInterfaceInstance(this);
       IVR_UILauncher.moveToThread( IVR_QtApp->instance()->thread() );
    }
}

uint IVRLowLevelSDK::IVR_GetCounter(uint pIVR_CamType)
{
    QFuture<uint> IVR_ShotThread = QtConcurrent::run([this,pIVR_CamType]()
    {
       uint Counter=0;
       for(auto i : IVRLowLevelSDK::IVR_CameraPool)
       {
           if(i->IVR_CameraType == pIVR_CamType)Counter++;
       }
       return Counter;
    });

    return IVR_ShotThread.result();
}

CIVRVirtualCamera *IVRLowLevelSDK::IVR_AddVirtualCam(uint    &pIVR_CameraId    ,
                                       uint     pIVR_CamType     ,
                                       uint     pIVR_RecMode     ,
                                       QString &pIVR_CameraName  ,
                                       qint64   pIVR_Timestamp)
{
    //A virtual Cam Collect everything that happens for a Render Camera in Unreal
    IVR_CameraPool.push_back(new CIVRVirtualCamera());
    pIVR_CameraId = IVR_CameraPool.size()-1;
    IVR_CameraPool[pIVR_CameraId]->IVR_CameraName    = pIVR_CameraName;
    IVR_CameraPool[pIVR_CameraId]->IVR_CameraType    = pIVR_CamType;
    IVR_CameraPool[pIVR_CameraId]->IVR_RecordingMode = pIVR_RecMode;
    IVR_CameraPool[pIVR_CameraId]->IVR_Timestamp     = pIVR_Timestamp;
    IVR_CameraPool[pIVR_CameraId]->IVR_RootFolder    = IVR_RootFolder;

    return IVR_CameraPool[pIVR_CameraId];
}

bool IVRLowLevelSDK::IVR_CheckRecordingState(uint pIVR_CameraId)
{
    QFuture<bool> IVR_ShotThread = QtConcurrent::run([this,pIVR_CameraId]()
    {
       return IVR_CameraPool[pIVR_CameraId]->IVR_IsRecording;
    });

    return IVR_ShotThread.result();
}


bool IVRLowLevelSDK::IVR_CleanUpRecordVSession()
{
    IVR_CameraPool.clear();
    IVR_CameraPool.shrink_to_fit();
    return true;
}

//------------------------------------------------------------
//GUI Thread Session
//------------------------------------------------------------
void IVRLowLevelSDK::IVR_FireUpGUIThread()
{
    // Code placed here will run in the game thread
    int   argc = 1;
    char* argv[2] = { (char*)"dummy", {} };

    IVR_QtApp = new QApplication(argc, argv);
    if(IVR_QtApp)
    {
        IVR_GUIActive = true;
        IVR_GUIThread = QtConcurrent::run(this,&IVRLowLevelSDK::IVR_FireGUIThread);
    }
}

void IVRLowLevelSDK::IVR_FireGUIThread()
{
    while(IVR_GUIActive)
    {
        IVR_QtApp->processEvents();
    }
}

void IVRLowLevelSDK::IVR_StopGUIThread()
{
    IVR_GUIActive = false;
    IVR_GUIThread.cancel();
}

bool IVRLowLevelSDK::IVR_IsGUIActive()
{
    return IVR_GUIActive;
}

//------------------------------------------------------------
//Processing Pipeline Session
//------------------------------------------------------------
void IVRLowLevelSDK::IVR_FireUpThreads()
{
    IVR_PipeActive = true;
    //IVR_RCThread = QtConcurrent::run(this,&IVRLowLevelSDK::IVR_CamRecLoopThread);
    //IVR_BFThread = QtConcurrent::run(this,&IVRLowLevelSDK::IVR_BufRecLoopThread);
}

void IVRLowLevelSDK::IVR_StopThreads()
{
    IVR_PipeActive = false;
    //IVR_RCThread.cancel();
}

bool IVRLowLevelSDK::IVR_IsThreadsActive()
{
    return IVR_PipeActive;
}
