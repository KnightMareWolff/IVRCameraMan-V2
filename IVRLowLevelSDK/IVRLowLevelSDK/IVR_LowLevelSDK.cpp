#include "IVR_LowLevelSDK.h"
#include "IVR_LauncherUI.h"

CIVRLauncherUI   IVR_UILauncher;

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

    if(IVR_RootFolder.isEmpty())return false;

    return true;
}

bool IVRLowLevelSDK::IVR_SetConfPath  (QString &pDAIAbsolutePath)
{
    IVR_ConfigFolder = QString(pDAIAbsolutePath);

    if(IVR_ConfigFolder.isEmpty())return false;

    return true;
}

void IVRLowLevelSDK::IVR_SetDebugMode(bool pIVR_DebbugMode)
{
    IVR_DebbugMode = pIVR_DebbugMode;
}

void IVRLowLevelSDK::IVR_SetWaitRecord(uint pIVR_MsecToWait     )
{
    IVR_MSecToWait = pIVR_MsecToWait;
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

bool IVRLowLevelSDK::IVR_AddVirtualCam(uint    &pIVR_CameraId    ,
                                       uint     pIVR_CamType     ,
                                       uint     pIVR_RecMode     ,
                                       QString &pIVR_CameraName  ,
                                       float    pIVR_FPS         ,
                                       bool     pIVR_IsStabilized,
                                       bool     pIVR_IsEnabled   ,
                                       qint64   pIVR_Timestamp)
{
    //A virtual Cam Collect everything that happens for a Render Camera in Unreal
    IVRLowLevelSDK::IVR_CameraPool.push_back(new CIVRVirtualCamera());
    pIVR_CameraId = IVR_CameraPool.size()-1;
    IVRLowLevelSDK::IVR_CameraPool[pIVR_CameraId]->IVR_CameraName    = pIVR_CameraName;
    IVRLowLevelSDK::IVR_CameraPool[pIVR_CameraId]->IVR_CameraType    = pIVR_CamType;
    IVRLowLevelSDK::IVR_CameraPool[pIVR_CameraId]->IVR_RecordingMode = pIVR_RecMode;
    IVRLowLevelSDK::IVR_CameraPool[pIVR_CameraId]->IVR_IsStabilized  = pIVR_IsStabilized;
    IVRLowLevelSDK::IVR_CameraPool[pIVR_CameraId]->IVR_DesiredFPS    = pIVR_FPS;
    IVRLowLevelSDK::IVR_CameraPool[pIVR_CameraId]->IVR_IsEnabled     = pIVR_IsEnabled;
    IVRLowLevelSDK::IVR_CameraPool[pIVR_CameraId]->IVR_Timestamp     = pIVR_Timestamp;

    return true;
}

bool IVRLowLevelSDK::IVR_UpdVirtualCam(uint              pIVR_CameraId   ,
                                       uint              pIVR_CamType    ,
                                       QString          &pIVR_CameraName ,
                                       float             pIVR_FPS        ,
                                       bool              pIVR_IsStabilized,
                                       bool              pIVR_IsEnabled)
{
    //A virtual Cam Collect everything that happens for a Render Camera in Unreal
    IVRLowLevelSDK::IVR_CameraPool[pIVR_CameraId]->IVR_CameraName   = pIVR_CameraName;
    IVRLowLevelSDK::IVR_CameraPool[pIVR_CameraId]->IVR_CameraType   = pIVR_CamType;
    IVRLowLevelSDK::IVR_CameraPool[pIVR_CameraId]->IVR_DesiredFPS   = pIVR_FPS;
    IVRLowLevelSDK::IVR_CameraPool[pIVR_CameraId]->IVR_IsStabilized = pIVR_IsStabilized;
    IVRLowLevelSDK::IVR_CameraPool[pIVR_CameraId]->IVR_IsEnabled    = pIVR_IsEnabled;
    return true;
}

bool IVRLowLevelSDK::IVR_CheckRecordingState(uint pIVR_CameraId)
{
    QFuture<bool> IVR_ShotThread = QtConcurrent::run([this,pIVR_CameraId]()
    {
       return IVR_CameraPool[pIVR_CameraId]->IVR_IsRecording;
    });

    return IVR_ShotThread.result();
}

//---------------------------------------------------------------------------------------------------------
//WARNING!
//---------------------------------------------------------------------------------------------------------
//This is an atomic operation being performed and controlled by Unreal, so it means all the Mutex
//AND Memory Allocations are being processed By Unreal, where our job here is take as fast we can
//the image data to be stored somewhere that Unreal cannot control anymore without impact on the
//Game/Recording
//
//This is the best way we have it to control the entire aplication, since we test many types of queues
//and algorithms to make possible make it work in different threads, but, the best and fast option was
//record this buffers in HD to be readed later.
//For other types of cameras like GamePlay Camera we will try allocate this on the component , but in
//another exclusive release.
//
//Simplifying, do not try optimize it using queues or other stuff, since we already made it and to threat
//IMAGE BUFFERS , keep it as an atomic operation. All the rest can be encapsulated on it without problems,
//ok? :)
//---------------------------------------------------------------------------------------------------------
bool IVRLowLevelSDK::IVR_RecordBuffer (IVR_RenderBuffer &pIVR_FrameBuffer)
{
    CIVRFrame  newFrame;

    newFrame.IVR_IsValid        = pIVR_FrameBuffer.IVR_IsValid;
    newFrame.IVR_CameraName     = pIVR_FrameBuffer.IVR_CameraName;
    newFrame.IVR_CameraType     = pIVR_FrameBuffer.IVR_CameraType;
    newFrame.IVR_Width          = pIVR_FrameBuffer.IVR_Width;
    newFrame.IVR_Height         = pIVR_FrameBuffer.IVR_Height;
    newFrame.IVR_ColorChannels  = pIVR_FrameBuffer.IVR_ColorChannels;
    newFrame.IVR_CameraID       = pIVR_FrameBuffer.IVR_CameraID;
    newFrame.IVR_FrameID        = pIVR_FrameBuffer.IVR_FrameID;
    newFrame.IVR_FrameFPS       = pIVR_FrameBuffer.IVR_FrameFPS;
    newFrame.IVR_FrameDT        = pIVR_FrameBuffer.IVR_FrameDT;
    newFrame.IVR_Position       = pIVR_FrameBuffer.IVR_Position;
    newFrame.IVR_Rotation       = pIVR_FrameBuffer.IVR_Rotation;
    newFrame.IVR_Scale          = pIVR_FrameBuffer.IVR_Scale;
    newFrame.IVR_Timestamp      = pIVR_FrameBuffer.IVR_Timestamp;

    IVR_CameraPool[pIVR_FrameBuffer.IVR_CameraID]->IVR_Frames.push_back(newFrame);

    int newIndex = IVR_CameraPool[pIVR_FrameBuffer.IVR_CameraID]->IVR_Frames.size()-1;

    IVR_CameraPool[pIVR_FrameBuffer.IVR_CameraID]->IVR_Frames[newIndex].IVR_FrameID = newIndex;
    pIVR_FrameBuffer.IVR_FrameID = newIndex;

    //--------------------------------------------------
    //Fill a new frame
    //--------------------------------------------------
    QString FileToWritePath;

    FileToWritePath = "/" + pIVR_FrameBuffer.IVR_CameraName + QString::fromStdString("_%1.jpg").arg(pIVR_FrameBuffer.IVR_FrameID);

    QString FinalPath        = IVR_RootFolder + FileToWritePath;
    //--------------------------------------------------------------
    //Record the FrameBuffer into HD.
    //--------------------------------------------------------------
    Mat tImageToRecord = Mat(pIVR_FrameBuffer.IVR_Height , pIVR_FrameBuffer.IVR_Width, CV_8UC4 , pIVR_FrameBuffer.IVR_Buffer.data());
    imwrite(FinalPath.toStdString(), tImageToRecord);

    return true;
}

//---------------------------------------------------------------------------------------------------------
//WARNING!
//---------------------------------------------------------------------------------------------------------
//Differently of the image buffers , the recording requests are native data and dont need big memory aloca
//tion to work, so we have it in a queue without problems and all the recordings are being fired in separa
//ted threads.
//
//Please note that this just put a recording message in a queue. the recording operation itself are happening
//On the recording Pipeline (An Infinite loop inside of its own thread, reading this queue). :)
//
//Thanks to Qt, to mantain the same syntax of Unreal AsyncTasks to call this threads in QtConcurrent! :)
//---------------------------------------------------------------------------------------------------------
bool IVRLowLevelSDK::IVR_HeadShotVideoRecord(uint pIVR_CameraId, uint pIVR_CamType)
{
    QFuture<void> IVR_ShotThread = QtConcurrent::run([this,pIVR_CameraId,pIVR_CamType]()
    {
        IVR_Task Task;

        Task.IVR_TaskType                 = IVR_Task_Type_Take;
        Task.IVR_CameraId                 = pIVR_CameraId;
        Task.IVR_CamType                  = pIVR_CamType;
        Task.IVR_RootFolder               = IVR_RootFolder;
        Task.IVR_FrameBuffer.IVR_CameraID = pIVR_CameraId;
        Task.IVR_FrameBuffer.IVR_IsValid  = false;

        IVR_RecordingPool.Push_Bf_Task(Task);
    });

    return true;
}

void IVRLowLevelSDK::IVR_RecordTake(uint pIVR_CameraId, QString IVR_RootFolder)
{
    QString IVR_FramesPath;
    QString IVR_VideoPath;

    //Record the video for each Camera we have
    uint FrameNumber      = 0;
    int frames_per_second = 0;
    uint rec_mode         = 0;

    //-------------------------------------------------------------------------
    //Check the recording smoothness...
    //-------------------------------------------------------------------------
    frames_per_second = IVR_CameraPool[pIVR_CameraId]->IVR_GetRecFPS();
    rec_mode = IVR_CameraPool[pIVR_CameraId]->IVR_RecordingMode;
    //If we not have frames to process stop here..
    if(frames_per_second==0)return;

    IVR_FramesPath = IVR_RootFolder + "/" + IVR_CameraPool[pIVR_CameraId]->IVR_CameraName + "_%d.jpg";
    IVR_VideoPath  = IVR_RootFolder + "/" + IVR_CameraPool[pIVR_CameraId]->IVR_CameraName + "Take" + QString("%1").arg(IVR_CameraPool[pIVR_CameraId]->IVR_Takes.size()) + ".mp4";

    IVR_CameraPool[pIVR_CameraId]->IVR_IsRecording=true;

    //-------------------------------------------------------------------------
    //Check the Recorded Frames and Open it in a Video Capture...
    //-------------------------------------------------------------------------
    VideoCapture cap(IVR_FramesPath.toStdString());

    // if not success, exit program
    if (cap.isOpened() == false)
    {
        //There is no recording for this camera, jump to the next one...
        return;
    }

    //-------------------------------------------------------------------------
    //Check the Video Capture Parameters and Codecs
    //-------------------------------------------------------------------------
    int frame_width  = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_WIDTH)); //get the width of frames of the video
    int frame_height = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_HEIGHT)); //get the height of frames of the video

    Size frame_size(frame_width, frame_height);

    //Create and initialize the VideoWriter object
    cv::VideoWriter oVideoWriter(IVR_VideoPath.toStdString(),
        cv::VideoWriter::fourcc('3', 'I', 'V', 'D'),
        frames_per_second,
        frame_size,
        true);

    //If the VideoWriter object is not initialized successfully, exit the program
    if (oVideoWriter.isOpened() == false)
    {
        //It´s not possible record this video!
        return;
    }

    //-------------------------------------------------------------------------
    //Start Record the Video
    //-------------------------------------------------------------------------
    while (true)
    {
        Mat frame;
        CIVRFrame ivrFrame;

        bool isSuccess = cap.read(frame); // read a new frame from the video camera

        //Breaking the while loop if frames cannot be read from the camera
        if (isSuccess == false)
        {
            frame.release();
            oVideoWriter.release();
            cap.release();
            break;
        }

        //Get the camera actual frame from vector
        ivrFrame = IVR_CameraPool[pIVR_CameraId]->IVR_Frames[FrameNumber];

        /*
        Make changes to the frame as necessary
        e.g.
        1. Change brightness/contrast of the image
        2. Smooth/Blur image
        3. Crop the image
        4. Rotate the image
        5. Draw shapes on the image
        */
        if(IVR_DebbugMode)
        {
           putText(frame, QString("FPS: %1").arg(frames_per_second).toStdString(),
                    cv::Point(frame.cols/12, frame.rows/12 + 10), //top-left position
                    cv::FONT_HERSHEY_DUPLEX,
                    1.0,
                    CV_RGB(0, 0, 0), //font color
                    2);
           putText(frame, QString("Elapsed: %1").arg(ivrFrame.IVR_FrameDT).toStdString(),
                    cv::Point(frame.cols/12 , frame.rows/12 + 60), //top-left position
                    cv::FONT_HERSHEY_DUPLEX,
                    1.0,
                    CV_RGB(0, 0, 0), //font color
                    2);

           QString strRecMode;
           switch(rec_mode)
           {
               case IVR_Recording_Mode_Average  :{strRecMode = QString("Average FPS");}break;
               case IVR_Recording_Mode_Best     :{strRecMode = QString("Best FPS");}break;
               case IVR_Recording_Mode_Desired  :{strRecMode = QString("Desired FPS");}break;
               case IVR_Recording_Mode_TimeAprox:{strRecMode = QString("Time Approximation");}break;
               case IVR_Recording_Mode_Stabilise:{strRecMode = QString("With Stabilization");}break;
               case IVR_Recording_Mode_SuperRes :{strRecMode = QString("With Super Resolution");}break;
           }
           putText(frame, QString(QString("Recording Mode: ") + strRecMode).toStdString(),
                    cv::Point(frame.cols/12 , frame.rows/12 + 110), //top-left position
                    cv::FONT_HERSHEY_DUPLEX,
                    1.0,
                    CV_RGB(0, 0, 0), //font color
                    2);
        }

        //write the video frame to the file
        oVideoWriter.write(frame);
        FrameNumber++;

    }

    //-------------------------------------------------------------------------
    //Clean Up the Camera Frames and Open a new Take
    //-------------------------------------------------------------------------
    IVR_CameraPool[pIVR_CameraId]->IVR_CameraTake(IVR_VideoPath);

    //CleanUp the Mess created with the frames...
    for (uint i = 0; i < FrameNumber; i++)
    {
        QString FileToDeletePath;

        FileToDeletePath = "/" + IVR_CameraPool[pIVR_CameraId]->IVR_CameraName + QString::fromStdString("_%1.jpg").arg(i);

        QString FinalPath        = IVR_RootFolder + FileToDeletePath;

        QFile FileToDelete(FinalPath);
        if (!FileToDelete.remove())
        {
            //if was not possible delete the file , something really strange happen...return a fail!
            return;
        }
    }

    //When we finish up a recording the render queue must be reset to receive new fresh buffers after a new recording start.
    IVR_CameraPool[pIVR_CameraId]->IVR_RenderQueue.IVR_BufferQueue->ResetCounter();

    //When we clean up and prepare everything we can consider the camera free for another recording.
    IVR_CameraPool[pIVR_CameraId]->IVR_IsRecording=false;
}


bool IVRLowLevelSDK::IVR_CleanUpRecordVSession()
{
    IVRLowLevelSDK::IVR_CameraPool.clear();
    IVRLowLevelSDK::IVR_CameraPool.shrink_to_fit();
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
    IVR_RCThread = QtConcurrent::run(this,&IVRLowLevelSDK::IVR_CamRecLoopThread);
}


void IVRLowLevelSDK::IVR_CamRecLoopThread()
{
    while(IVR_PipeActive)
    {
        IVR_Task t1 = IVR_RecordingPool.Pop_Bf_Task();
        if(t1.IVR_CameraId != 99999 && t1.IVR_CamType !=99999)
        {
           std::this_thread::sleep_for(std::chrono::milliseconds(IVR_MSecToWait));
           QFuture<void> IVR_RecTakeThread = QtConcurrent::run(this,&IVRLowLevelSDK::IVR_RecordTake,t1.IVR_CameraId,IVR_RootFolder);
        }
    }
}


void IVRLowLevelSDK::IVR_StopThreads()
{
    IVR_PipeActive = false;
    IVR_RCThread.cancel();
}

bool IVRLowLevelSDK::IVR_IsThreadsActive()
{
    return IVR_PipeActive;
}
