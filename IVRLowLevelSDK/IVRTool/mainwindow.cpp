#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    IVRLowLevelSDK* pInterf = new IVRLowLevelSDK();

    uint CamId;
    uint CamType=0;
    QString pCamName = "Test";

    IVR_RenderBuffer pBuffer;
    IVR_RenderBuffer pBuffer1;
    IVR_RenderBuffer pBuffer2;
    IVR_RenderBuffer pBuffer3;

    pBuffer.IVR_Width          = 1024;
    pBuffer.IVR_Height         = 768;
    pBuffer.IVR_ColorChannels  = 4;
    pBuffer.IVR_CameraID       = 0;
    pBuffer.IVR_FrameID        = 1;
    pBuffer.IVR_FrameFPS       = 60;
    pBuffer.IVR_Position       = QVector3D(0,0,0);
    pBuffer.IVR_Rotation       = QVector3D(0,0,0);
    pBuffer.IVR_Scale          = QVector3D(0,0,0);
    pBuffer.IVR_Timestamp      = 20090520145024798;
    pBuffer.IVR_Buffer         = QByteArray("000000000000000000000000000000000000000000000000000000000000000000000000000000000");

    pBuffer1.IVR_Width          = 1024;
    pBuffer1.IVR_Height         = 768;
    pBuffer1.IVR_ColorChannels  = 4;
    pBuffer1.IVR_CameraID       = 0;
    pBuffer1.IVR_FrameID        = 1;
    pBuffer1.IVR_FrameFPS       = 60;
    pBuffer1.IVR_Position       = QVector3D(0,0,0);
    pBuffer1.IVR_Rotation       = QVector3D(0,0,0);
    pBuffer1.IVR_Scale          = QVector3D(0,0,0);
    pBuffer1.IVR_Timestamp      = 20100520145024798;
    pBuffer1.IVR_Buffer         = QByteArray("000000000000000000000000000000000000000000000000000000000000000000000000000000000");

    pBuffer2.IVR_Width          = 1024;
    pBuffer2.IVR_Height         = 768;
    pBuffer2.IVR_ColorChannels  = 4;
    pBuffer2.IVR_CameraID       = 0;
    pBuffer2.IVR_FrameID        = 1;
    pBuffer2.IVR_FrameFPS       = 60;
    pBuffer2.IVR_Position       = QVector3D(0,0,0);
    pBuffer2.IVR_Rotation       = QVector3D(0,0,0);
    pBuffer2.IVR_Scale          = QVector3D(0,0,0);
    pBuffer2.IVR_Timestamp      = 20110520145024798;
    pBuffer2.IVR_Buffer         = QByteArray("000000000000000000000000000000000000000000000000000000000000000000000000000000000");

    pBuffer3.IVR_Width          = 1024;
    pBuffer3.IVR_Height         = 768;
    pBuffer3.IVR_ColorChannels  = 4;
    pBuffer3.IVR_CameraID       = 0;
    pBuffer3.IVR_FrameID        = 1;
    pBuffer3.IVR_FrameFPS       = 60;
    pBuffer3.IVR_Position       = QVector3D(0,0,0);
    pBuffer3.IVR_Rotation       = QVector3D(0,0,0);
    pBuffer3.IVR_Scale          = QVector3D(0,0,0);
    pBuffer3.IVR_Timestamp      = 20120520145024798;
    pBuffer3.IVR_Buffer         = QByteArray("000000000000000000000000000000000000000000000000000000000000000000000000000000000");

    pInterf->IVR_AddVirtualCam(CamId,CamType,0,pCamName,0.5f,false,false,0);
    /*
    IVR_CMRenderQueue.push_back(new CIVRTaskQueue());

    IVR_BuffTask Task0;

    Task0.pIVR_ImageBuffer = new IVR_RenderBuffer(pBuffer);
    Task0.pIVR_CamType     = CamType;
    Task0.IVR_RootFolder   = "TestFolder";

    IVR_CMRenderQueue[0]->Push_Bf_Task(Task0);

    IVR_BuffTask Task1;

    Task1.pIVR_ImageBuffer = new IVR_RenderBuffer(pBuffer1);
    Task1.pIVR_CamType     = CamType;
    Task1.IVR_RootFolder   = "TestFolder";

    IVR_CMRenderQueue[0]->Push_Bf_Task(Task1);

    IVR_BuffTask Task2;

    Task2.pIVR_ImageBuffer = new IVR_RenderBuffer(pBuffer2);
    Task2.pIVR_CamType     = CamType;
    Task2.IVR_RootFolder   = "TestFolder";

    IVR_CMRenderQueue[0]->Push_Bf_Task(Task2);

    IVR_BuffTask Task3;

    Task3.pIVR_ImageBuffer = new IVR_RenderBuffer(pBuffer3);
    Task3.pIVR_CamType     = CamType;
    Task3.IVR_RootFolder   = "TestFolder";

    IVR_CMRenderQueue[0]->Push_Bf_Task(Task3);

    IVR_BuffTask t0 = IVR_CMRenderQueue[0]->Pop_Bf_Task();
    IVR_BuffTask t1 = IVR_CMRenderQueue[0]->Pop_Bf_Task();
    IVR_BuffTask t2 = IVR_CMRenderQueue[0]->Pop_Bf_Task();
    IVR_BuffTask t3 = IVR_CMRenderQueue[0]->Pop_Bf_Task();
    */


    CIVRVirtualCamera *tCam = new CIVRVirtualCamera();

    CIVRFrame tFrame;
    tFrame.IVR_FrameFPS = 45;

    for(int i=0;i<7;i++)
    {
        if(i<3)tFrame.IVR_FrameFPS = 35;
        if(i>=3)tFrame.IVR_FrameFPS = 60;

        tCam->IVR_Frames.push_back(tFrame);

        IVR_Task ttask;
        ttask.IVR_TaskType    = IVR_Task_Type_Take;
        ttask.IVR_CameraId    = i;
        ttask.IVR_CamType     = i;
        ttask.IVR_RootFolder  = "IVR_RootFolder";

        ttask.IVR_FrameBuffer.IVR_CameraID = i;
        ttask.IVR_FrameBuffer.IVR_IsValid=true;
        ttask.IVR_FrameBuffer.IVR_Timestamp=QDateTime::currentMSecsSinceEpoch();

        tCam->IVR_RenderQueue.Push_Bf_Task(ttask);
        qWarning() << "Incluiu: " << ttask.IVR_FrameBuffer.IVR_FrameID;
    }

    IVR_Task ttask;
    for(int i=0;i<9;i++)
    {
        ttask.ClearTask();
        ttask=tCam->IVR_RenderQueue.Pop_Bf_Task();
        if(ttask.IVR_CamType != 99999)
        {
        qWarning() << "Retirou: " << ttask.IVR_FrameBuffer.IVR_FrameID;
        qWarning() << "Timestamp:" << ttask.IVR_FrameBuffer.IVR_Timestamp;
        }
    }

    //int best = tCam->IVR_GetBestFPS();
    //qWarning() << "Best FPS: " << best;

    pInterf->IVR_SpawnStage();
    qWarning() << "Huhuuu!";
}

MainWindow::~MainWindow()
{
}

