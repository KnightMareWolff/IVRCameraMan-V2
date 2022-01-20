#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    IVRLowLevelSDK* pInterf = new IVRLowLevelSDK();

    uint CamId;
    uint CamType=0;
    QString pCamName = "Test";

    LockFreeQueue<IVR_RenderBuffer,10000> IVR_CMRenderQueue;
    IVR_RenderBuffer pBuffer;
    IVR_RenderBuffer pBuffer1;
    IVR_RenderBuffer pBuffer2;
    IVR_RenderBuffer pBuffer3;

    pBuffer.IVR_Width          = 1024;
    pBuffer.IVR_Height         = 768;
    pBuffer.IVR_ColorChannels  = 4;

    //pBuffer.IVR_Buffer         = QByteArray("000000000000000000000000000000000000000000000000000000000000000000000000000000000");


    pBuffer1.IVR_Width          = 1024;
    pBuffer1.IVR_Height         = 768;
    pBuffer1.IVR_ColorChannels  = 4;

    //pBuffer1.IVR_Buffer         = QByteArray("000000000000000000000000000000000000000000000000000000000000000000000000000000000");


    pBuffer2.IVR_Width          = 1024;
    pBuffer2.IVR_Height         = 768;
    pBuffer2.IVR_ColorChannels  = 4;

    //pBuffer2.IVR_Buffer         = QByteArray("000000000000000000000000000000000000000000000000000000000000000000000000000000000");


    pBuffer3.IVR_Width          = 1024;
    pBuffer3.IVR_Height         = 768;
    pBuffer3.IVR_ColorChannels  = 4;

    //pBuffer3.IVR_Buffer         = QByteArray("000000000000000000000000000000000000000000000000000000000000000000000000000000000");

    pInterf->IVR_AddVirtualCam(CamId,CamType,0,pCamName,0);

    IVR_CMRenderQueue.push(pBuffer);
    IVR_CMRenderQueue.push(pBuffer1);
    IVR_CMRenderQueue.push(pBuffer2);
    IVR_CMRenderQueue.push(pBuffer3);

    auto p1 = IVR_CMRenderQueue.front();
    IVR_CMRenderQueue.pop();

    auto p2 = IVR_CMRenderQueue.front();
    IVR_CMRenderQueue.pop();

    auto p3 = IVR_CMRenderQueue.front();
    IVR_CMRenderQueue.pop();

    auto p4 = IVR_CMRenderQueue.front();
    IVR_CMRenderQueue.pop();

    auto p5 = IVR_CMRenderQueue.front();
    IVR_CMRenderQueue.pop();

    auto p6 = IVR_CMRenderQueue.front();
    IVR_CMRenderQueue.pop();



    pInterf->IVR_SpawnStage();
    qWarning() << "Huhuuu!";
}

MainWindow::~MainWindow()
{
}

