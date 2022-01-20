#include "IVR_VirtualCamera.h"

CIVRVirtualCamera::CIVRVirtualCamera()
{
   IVR_RecordingMode = IVR_Recording_Mode_Best;
   IVR_DesiredFPS    = 60;
   IVR_IsRecording   = false;
   IVR_ActualTake    = 0;
   IVR_haveOpenTake  = false;
   IVR_IsStabilized  = false;
   IVR_IsCompiling   = false;

   if(!IVR_haveOpenTake)
   {
       CIVRCameraTake pTake;

       pTake.IVR_TakeID = IVR_Takes.size();

       IVR_ActualTake = pTake.IVR_TakeID;

       IVR_Takes.push_back(pTake);

       IVR_haveOpenTake = true;
   }

   IVR_RenderQueue = nullptr;
   IVR_DataQueue   = nullptr;
}

uint CIVRVirtualCamera::IVR_GetRecFPS()
{
    switch(IVR_RecordingMode)
    {
        case IVR_Recording_Mode_Average  :{return IVR_GetAverageFPS();}break;
        case IVR_Recording_Mode_Best     :{return IVR_GetBestFPS();}break;
        case IVR_Recording_Mode_Desired  :{return IVR_GetDesiredFPS();}break;
        case IVR_Recording_Mode_TimeAprox:{return IVR_GetFPSByTimeApprox();}break;
        case IVR_Recording_Mode_Stabilise:{return IVR_GetFPSByTimeApprox();}break;
        case IVR_Recording_Mode_SuperRes :{return IVR_GetFPSByTimeApprox();}break;
    }
    return 0;
}

uint CIVRVirtualCamera::IVR_GetAverageFPS()
{

    int FrameIndex=0;
    bool enOfFile = false;

    QString IVR_FrameFile  = IVR_RootFolder + "/" + IVR_CameraName + "Take" + QString("%1").arg(IVR_ActualTake) + ".iff";

    //---------------------------------------------------------------------
    //Open the frame file and fill the camera Frames
    //---------------------------------------------------------------------
    FILE *frameFile;

    frameFile = fopen(IVR_FrameFile.toStdString().c_str(),"r");

    uint tFramesFPS = 0;

    if (frameFile != NULL)
    {
        while(!enOfFile)
        {
           IVR_FrameData tFrame;

           if(fread(&tFrame.IVR_CameraType , sizeof(uint  ), 1, frameFile) == 1 &&
              fread(&tFrame.IVR_CameraID   , sizeof(uint  ), 1, frameFile) == 1 &&
              fread(&tFrame.IVR_FrameFPS   , sizeof(uint  ), 1, frameFile) == 1 &&
              fread(&tFrame.IVR_FrameDT    , sizeof(float ), 1, frameFile) == 1 &&
              fread(&tFrame.IVR_Timestamp  , sizeof(qint64), 1, frameFile) == 1 )
           {


               tFrame.IVR_FrameID    = FrameIndex;

               tFramesFPS += tFrame.IVR_FrameFPS;

               FrameIndex++;
           }
           else
           {
               fclose(frameFile);
               enOfFile = true;
           }
        }
    }
    else
    {
        qWarning() << "Problems to open the Frame File!";
        return 0;
    }

    tFramesFPS = tFramesFPS / (FrameIndex + 1);

    qWarning() << "Averaged FPS Identifyed: " << tFramesFPS ;

    return tFramesFPS;
}

uint CIVRVirtualCamera::IVR_GetBestFPS()
{
    int FrameIndex=0;
    bool enOfFile = false;
    //uint BestFPS;

    QString IVR_FrameFile  = IVR_RootFolder + "/" + IVR_CameraName + "Take" + QString("%1").arg(IVR_ActualTake) + ".iff";

    // temporary structure to make possible we sort the axis vector
    typedef struct fpsVal
    {
        int   index;
        float ocurrencies;
    }fpsVal;

    // sort by the less distance
    struct
    {
         bool operator()(fpsVal a, fpsVal b) const
         {
             return a.ocurrencies > b.ocurrencies;
         }
    } GreaterOccours;

    vector<float>  fpsFloatData;
    vector<fpsVal> fpsOccurenciesData;

    //---------------------------------------------------------------------
    //Open the frame file and fill the camera Frames
    //---------------------------------------------------------------------
    FILE *frameFile;

    frameFile = fopen(IVR_FrameFile.toStdString().c_str(),"rb");

    if (frameFile != NULL)
    {
        while(!enOfFile)
        {
           IVR_FrameData tFrame;

           if(fread(&tFrame.IVR_CameraType , sizeof(uint  ), 1, frameFile) == 1 &&
              fread(&tFrame.IVR_CameraID   , sizeof(uint  ), 1, frameFile) == 1 &&
              fread(&tFrame.IVR_FrameFPS   , sizeof(uint  ), 1, frameFile) == 1 &&
              fread(&tFrame.IVR_FrameDT    , sizeof(float ), 1, frameFile) == 1 &&
              fread(&tFrame.IVR_Timestamp  , sizeof(qint64), 1, frameFile) == 1 )
           {

               tFrame.IVR_FrameID    = FrameIndex;

               fpsFloatData.push_back((float)tFrame.IVR_FrameFPS);

               FrameIndex++;
           }
           else
           {
               fclose(frameFile);
               enOfFile = true;
           }
        }
    }
    else
    {
        qWarning() << "Problems to open the Frame File!";
        return 0;
    }


    Mat fpsData( 1 , FrameIndex + 1 , CV_32F , &fpsFloatData[0]);
    Mat hist;
    int bins = 120;
    int histSize[] = { bins };
    float range[] = { 0, 120 };
    const float* ranges[] = { range };
    int channels[] = {0, 1};

    calcHist( &fpsData, 1, channels , Mat(), hist,1, histSize, ranges, true,false );

    for(int i=0;i<120;i++)
    {
       float hval = hist.at<float>(i,0);
       if(hval>0)
       {
           fpsVal tVal;
           tVal.index=i;
           tVal.ocurrencies=hval;
           fpsOccurenciesData.push_back(tVal);
       }
    }

    //Sort the final vector taking account the FPS occurencies
    std::sort(fpsOccurenciesData.begin(), fpsOccurenciesData.end(), GreaterOccours);

    //The Best FPS :)
    return fpsOccurenciesData[0].index;
}

uint CIVRVirtualCamera::IVR_GetDesiredFPS()
{
    return IVR_DesiredFPS;
}

uint CIVRVirtualCamera::IVR_GetFPSByTimeApprox()
{
    uint FPS     = 60;
    uint nFrames = 1;
    int  milisec = 1.000;
    int FrameIndex=0;

    QString IVR_FrameFile  = IVR_RootFolder + "/" + IVR_CameraName + "Take" + QString("%1").arg(IVR_ActualTake) + ".iff";

    QDateTime pIVR_TakeInitialTime;
    QDateTime pIVR_TakeFinalTime;

    //---------------------------------------------------------------------
    //Open the frame file and fill the camera Frames
    //---------------------------------------------------------------------
    FILE *frameFile;

    frameFile = fopen(IVR_FrameFile.toStdString().c_str(),"rb");

    if (frameFile != NULL)
    {
        while(true)
        {
           IVR_FrameData tFrame;

           if(fread(&tFrame.IVR_CameraType , sizeof(uint  ), 1, frameFile) == 1 &&
              fread(&tFrame.IVR_CameraID   , sizeof(uint  ), 1, frameFile) == 1 &&
              fread(&tFrame.IVR_FrameFPS   , sizeof(uint  ), 1, frameFile) == 1 &&
              fread(&tFrame.IVR_FrameDT    , sizeof(float ), 1, frameFile) == 1 &&
              fread(&tFrame.IVR_Timestamp  , sizeof(qint64), 1, frameFile) == 1 )
           {

               tFrame.IVR_FrameID    = FrameIndex;

               if(FrameIndex==0)
               {
                   pIVR_TakeInitialTime = QDateTime::fromMSecsSinceEpoch(tFrame.IVR_Timestamp);
               }

               FrameIndex++;
           }
           else
           {
               pIVR_TakeFinalTime = QDateTime::fromMSecsSinceEpoch(tFrame.IVR_Timestamp);

               fclose(frameFile);
               break;
           }
        }
    }
    else
    {
        qWarning() << "Problems to open the Frame File!";
        return 0;
    }

    QDateTime pIVR_Elapsed;
    quint64   pIVR_TakeElapsedTime = 0;

    pIVR_Elapsed.fromMSecsSinceEpoch(pIVR_TakeInitialTime.msecsTo(pIVR_TakeFinalTime));

    pIVR_TakeElapsedTime = pIVR_Elapsed.toTime_t();

    FPS     = IVR_GetBestFPS();

    nFrames = FrameIndex + 1;

    qWarning() << "FPS Identifyed for Time Approx: " << FPS ;
    qWarning() << "Number of Frames Identifyed for Time Approx: " << nFrames ;
    qWarning() << "System Initial Time: " << pIVR_TakeInitialTime ;
    qWarning() << "System Final   Time: " << pIVR_TakeFinalTime   ;
    qWarning() << "System Elapsed Time: " << pIVR_TakeElapsedTime ;

    quint64 actualElapsed = 0;
    //This is the system actual resultant video Time(How much longer it play)
    if(FPS != 0 && nFrames > 0)
    {
       actualElapsed = ((nFrames / FPS) * milisec);
    }

    qWarning() << "Calculated Elapsed Time: " << actualElapsed ;

    //TODO:
    //Check if the timestamped time match with the calculated time and try approximate them
    qWarning() << "Final FPS Identifyed: " << FPS << "Final Elapsed Time: " << actualElapsed;

    return FPS;
}


bool CIVRVirtualCamera::IVR_RecordBuffer(const IVR_RenderBuffer &pIVR_FrameBuffer)
{
    if(!IVR_IsRecording )return false;
    if(!IVR_haveOpenTake)return false;
    if( IVR_IsCompiling )return false;

    QFuture<void> IVR_UNThread = QtConcurrent::run([this,pIVR_FrameBuffer]()
    {
       IVR_RenderQueue->push(pIVR_FrameBuffer);
    });

    return true;
}

bool CIVRVirtualCamera::IVR_RecordData(const IVR_FrameData &pIVR_FrameData)
{
    if(!IVR_IsRecording )return false;
    if(!IVR_haveOpenTake)return false;
    if( IVR_IsCompiling )return false;

    IVR_DataQueue->push(pIVR_FrameData);
    return true;
}

void CIVRVirtualCamera::IVR_DataLoop()
{
    while(IVR_IsRecording)
    {
            auto data = IVR_DataQueue->front();
            if(data.IVR_IsValid)
            {
               //---------------------------------------------------------------------
               //Append in the frames file a single realtime frame
               //---------------------------------------------------------------------
               FILE *frameFile;

               frameFile = fopen(QString(IVR_RootFolder + "/" + IVR_CameraName + "Take" + QString("%1").arg(IVR_ActualTake) + ".iff").toStdString().c_str(),"ab");

               if (frameFile != NULL)
               {
                   fwrite(&data.IVR_CameraType, sizeof(uint  ), 1, frameFile);
                   fwrite(&data.IVR_CameraID  , sizeof(uint  ), 1, frameFile);
                   fwrite(&data.IVR_FrameFPS  , sizeof(uint  ), 1, frameFile);
                   fwrite(&data.IVR_FrameDT   , sizeof(float ), 1, frameFile);
                   fwrite(&data.IVR_Timestamp , sizeof(qint64), 1, frameFile);
               }

               fclose(frameFile);

               IVR_DataQueue->clean();
               IVR_DataQueue->pop();
            }
    }

    if(IVR_DataQueue->size()>0)
    {

    while(IVR_DataQueue->size()>0)
    {
            auto data = IVR_DataQueue->front();
            if(data.IVR_IsValid)
            {
               //---------------------------------------------------------------------
               //Append in the frames file a single realtime frame
               //---------------------------------------------------------------------
               FILE *frameFile;

               frameFile = fopen(QString(IVR_RootFolder + "/" + IVR_CameraName + "Take" + QString("%1").arg(IVR_ActualTake) + ".iff").toStdString().c_str(),"ab");

               if (frameFile != NULL)
               {
                   fwrite(&data.IVR_CameraType, sizeof(uint  ), 1, frameFile);
                   fwrite(&data.IVR_CameraID  , sizeof(uint  ), 1, frameFile);
                   fwrite(&data.IVR_FrameFPS  , sizeof(uint  ), 1, frameFile);
                   fwrite(&data.IVR_FrameDT   , sizeof(float ), 1, frameFile);
                   fwrite(&data.IVR_Timestamp , sizeof(qint64), 1, frameFile);
               }

               fclose(frameFile);

               IVR_DataQueue->clean();
               IVR_DataQueue->pop();
            }
    }

    }
}

void CIVRVirtualCamera::IVR_ImageLoop()
{
    while(IVR_IsRecording)
    {
            //---------------------------------------------------
            //Buffer Processing
            //---------------------------------------------------
            auto buffer = IVR_RenderQueue->front();

            if(buffer.IVR_Width > 0 && buffer.IVR_Height > 0 && buffer.IVR_ColorChannels >0)
            {
               //---------------------------------------------------------------------
               //Append in the images file a single realtime framebuffer
               //---------------------------------------------------------------------
               FILE *imageFile;

               imageFile = fopen(QString(IVR_RootFolder + "/" +
                                         IVR_CameraName + "Take" +
                                         QString("%1").arg(IVR_ActualTake) +
                                         QString("Frm%1").arg(IVR_FrameCounter) +
                                         ".iif").toStdString().c_str(),"ab");

               if (imageFile != NULL)
               {

                   fwrite(&buffer.IVR_Width        , sizeof(int), 1, imageFile);
                   fwrite(&buffer.IVR_Height       , sizeof(int), 1, imageFile);
                   fwrite(&buffer.IVR_ColorChannels, sizeof(int), 1, imageFile);

                   fwrite(buffer.IVR_Buffer.data , buffer.IVR_Width * buffer.IVR_Height * buffer.IVR_ColorChannels , 1 , imageFile);
               }

               fclose(imageFile);

               IVR_FrameCounter++;

               buffer.IVR_Buffer.release();
               buffer.IVR_Buffer = Mat();

               IVR_RenderQueue->clean();
               IVR_RenderQueue->pop();
            }
    }

    if(IVR_RenderQueue->size()>0)
    {
    while(IVR_RenderQueue->size()>0)
    {
            //---------------------------------------------------
            //Buffer Processing
            //---------------------------------------------------
            auto buffer = IVR_RenderQueue->front();

            if(buffer.IVR_Width > 0 && buffer.IVR_Height > 0 && buffer.IVR_ColorChannels >0)
            {
               //---------------------------------------------------------------------
               //Append in the images file a single realtime framebuffer
               //---------------------------------------------------------------------
               FILE *imageFile;

               imageFile = fopen(QString(IVR_RootFolder + "/" +
                                         IVR_CameraName + "Take" +
                                         QString("%1").arg(IVR_ActualTake) +
                                         QString("Frm%1").arg(IVR_FrameCounter) +
                                         ".iif").toStdString().c_str(),"ab");

               if (imageFile != NULL)
               {

                   fwrite(&buffer.IVR_Width        , sizeof(int), 1, imageFile);
                   fwrite(&buffer.IVR_Height       , sizeof(int), 1, imageFile);
                   fwrite(&buffer.IVR_ColorChannels, sizeof(int), 1, imageFile);

                   fwrite(buffer.IVR_Buffer.data , buffer.IVR_Width * buffer.IVR_Height * buffer.IVR_ColorChannels , 1 , imageFile);
               }

               fclose(imageFile);

               IVR_FrameCounter++;

               buffer.IVR_Buffer.release();
               buffer.IVR_Buffer = Mat();

               IVR_RenderQueue->clean();
               IVR_RenderQueue->pop();
            }
    }
    }
}

bool CIVRVirtualCamera::IVR_StartRecord()
{
    //If we are already recording and somehow the high-level layer leave it pass... :(
    if( IVR_IsRecording )return false;
    if(!IVR_haveOpenTake)return false;
    if( IVR_IsCompiling )return false;

    IVR_RenderQueue = new LockFreeQueue<IVR_RenderBuffer, 6720 >();
    IVR_DataQueue   = new LockFreeQueue<IVR_FrameData   , 2880  >();

    if(!IVR_RenderQueue || !IVR_DataQueue)return false;

    IVR_IsRecording = true;
    IVR_FrameCounter= 0;

    qWarning() << "Starting Data Loop";
    IVR_DTThread = QtConcurrent::run(this,&CIVRVirtualCamera::IVR_DataLoop);
    IVR_BFThread = QtConcurrent::run(this,&CIVRVirtualCamera::IVR_ImageLoop);

    return true;
}

bool CIVRVirtualCamera::IVR_StopRecord()
{
    IVR_IsRecording = false;
    return true;
}

bool CIVRVirtualCamera::IVR_CompileTake()
{
    QFuture<void> IVR_CPThread = QtConcurrent::run([this]()
    {
        //Waits for the recording Threads Stop
        while(IVR_BFThread.isRunning());
        while(IVR_DTThread.isRunning());

        CIVRCameraTake pTake;
        QString        IVR_VideoPath;
        QString        IVR_FrameFile;
        QString        IVR_ImageFile;

        //Record the video for each Camera we have
        uint FrameNumber      = 0;
        int frames_per_second = 0;
        uint rec_mode         = 0;

        //Open the Image file To record The Video Frame
        FILE *imageFile;

        //-------------------------------------------------------------------------
        //Initial Recording Setup
        //-------------------------------------------------------------------------
        //Block to receive more frames for this camera
        IVR_IsCompiling=true;

        //Free Up all the render queues since we dont know when we will start record again...
        delete IVR_RenderQueue;
        delete IVR_DataQueue  ;
        IVR_RenderQueue = nullptr;
        IVR_DataQueue   = nullptr;

        IVR_VideoPath  = IVR_RootFolder + "/" + IVR_CameraName + "Take" + QString("%1").arg(IVR_ActualTake) + ".mp4";
        IVR_FrameFile  = IVR_RootFolder + "/" + IVR_CameraName + "Take" + QString("%1").arg(IVR_ActualTake) + ".iff";

        IVR_Takes[IVR_ActualTake].IVR_VideoTake = IVR_VideoPath;

        //-------------------------------------------------------------------------
        //Check the recording smoothness...
        //-------------------------------------------------------------------------
        frames_per_second = IVR_GetRecFPS();

        //If we not have frames to process stop here..
        if(frames_per_second==0)return;

        //Set recording Mode...
        rec_mode  = IVR_RecordingMode;

        //-------------------------------------------------------------------------
        //Check the Video Capture Parameters and Codecs
        //-------------------------------------------------------------------------
        int frame_width  = static_cast<int>(CIVRConfig::IVR_Width );
        int frame_height = static_cast<int>(CIVRConfig::IVR_Height);

        Size frame_size(frame_width, frame_height);

        qWarning() << "======================================================";
        qWarning() << "Starting Record Video : " << IVR_VideoPath;
        qWarning() << "======================================================";
        qWarning() << "The Recording Parameters are:";
        qWarning() << "Camera Name: " <<  IVR_CameraName;
        qWarning() << "FPS        : " <<  frames_per_second;
        qWarning() << "Rec Mode   : " <<  rec_mode;
        qWarning() << "Width      : " <<  frame_width;
        qWarning() << "Height     : " <<  frame_height;
        qWarning() << "Frames Read: " <<  IVR_FrameCounter;
        qWarning() << "======================================================";

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
            qWarning() << "Fail to open Video File!";
            return;
        }

        IVR_RenderBuffer ivrBuffer;
        char             *pImage=nullptr;
        //-------------------------------------------------------------------------
        //Start Record the Video
        //-------------------------------------------------------------------------
        while (true)
        {
            IVR_ImageFile  = IVR_RootFolder + "/" +
                             IVR_CameraName + "Take" +
                             QString("%1").arg(IVR_ActualTake) +
                             QString("Frm%1").arg(FrameNumber) +
                             ".iif";

            imageFile = fopen(IVR_ImageFile.toStdString().c_str(),"rb");
            if (imageFile == NULL)
            {
                qWarning() << "Finishing the Video!";

                //Clear the Buffer Memory
                //if(ivrBuffer.IVR_Buffer.total()>0)ivrBuffer.IVR_Buffer = Mat();
                //ivrBuffer.IVR_Buffer = Mat();

                //Clear the Load Auxiliary Memory
                //if(pImage)
                //{
                //    delete []  pImage;
                //    pImage = nullptr;
                //}

                //Finish Write the Final Video
                oVideoWriter.release();
                break;
            }

            //Load the frame data
            if (fread(&ivrBuffer.IVR_Width        , sizeof(int), 1, imageFile) == 1 &&
                fread(&ivrBuffer.IVR_Height       , sizeof(int), 1, imageFile) == 1 &&
                fread(&ivrBuffer.IVR_ColorChannels, sizeof(int), 1, imageFile) == 1 )
            {
                //create the necessary loading memory
                if(!pImage)
                {
                   pImage = new char[ivrBuffer.IVR_Width * ivrBuffer.IVR_Height * ivrBuffer.IVR_ColorChannels];
                }

                //Load The Image from Disk
                if(fread(pImage , ivrBuffer.IVR_Width * ivrBuffer.IVR_Height * ivrBuffer.IVR_ColorChannels , 1, imageFile) != 1)
                {
                    qWarning() << "Fail to read Image Buffer!";
                    break;
                }

                //Clean if necessary and Fill the Image Buffer
                if(ivrBuffer.IVR_Buffer.total()>0)ivrBuffer.IVR_Buffer = Mat();
                ivrBuffer.IVR_Buffer = Mat(ivrBuffer.IVR_Height , ivrBuffer.IVR_Width, CV_8UC4 , pImage);
            }
            else
            {
                qWarning() << QString("Invalid Frame Data at Frame: %1").arg(FrameNumber);
                FrameNumber++;
                continue;
            }

            /*
            Make changes to the frame as necessary
            e.g.
            1. Change brightness/contrast of the image
            2. Smooth/Blur image
            3. Crop the image
            4. Rotate the image
            5. Draw shapes on the image
            */

            if(CIVRConfig::IVR_DebbugMode)
            {
               putText(ivrBuffer.IVR_Buffer, QString("FPS: %1").arg(frames_per_second).toStdString(),
                        cv::Point(ivrBuffer.IVR_Buffer.cols/12, ivrBuffer.IVR_Buffer.rows/12 + 10), //top-left position
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
               putText(ivrBuffer.IVR_Buffer, QString(QString("Recording Mode: ") + strRecMode).toStdString(),
                        cv::Point(ivrBuffer.IVR_Buffer.cols/12 , ivrBuffer.IVR_Buffer.rows/12 + 50), //top-left position
                        cv::FONT_HERSHEY_DUPLEX,
                        1.0,
                        CV_RGB(0, 0, 0), //font color
                        2);
            }

            //write the video frame to the file
            oVideoWriter.write(ivrBuffer.IVR_Buffer);

            //Close the Image File
            fclose(imageFile);
            QFile deleteImageFile(IVR_ImageFile);
            deleteImageFile.remove();

            //Clean-Up the auxiliary memory for the next frame
            delete []  pImage;
            pImage = nullptr;

            FrameNumber++;

        }

        //-------------------------------------------------------------------------
        //Final Recording Setup
        //-------------------------------------------------------------------------
        //When we finish record a take, we already open a new one to avoid unecessary processing at buffer receive.
        IVR_haveOpenTake = false;
        if(!IVR_haveOpenTake)
        {
            CIVRCameraTake pTake;

            pTake.IVR_TakeID = IVR_Takes.size();

            IVR_ActualTake = pTake.IVR_TakeID;

            IVR_Takes.push_back(pTake);

            IVR_haveOpenTake = true;
        }

        QFile deleteFrameFile(IVR_FrameFile);
        deleteFrameFile.remove();

        //Free to receive more frames for this camera
        IVR_FrameCounter = 0;
        IVR_IsCompiling  = false;

    });

    return true;
}
