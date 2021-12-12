#include "IVR_VirtualCamera.h"

CIVRVirtualCamera::CIVRVirtualCamera()
{
   IVR_RecordingMode = IVR_Recording_Mode_Best;
   IVR_DesiredFPS    = 60;
   IVR_IsRecording   = false;
}

int CIVRVirtualCamera::IVR_GetRecFPS()
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
int CIVRVirtualCamera::IVR_GetAverageFPS()
{
    //If we not have Frames to average...
    if(IVR_Frames.size()==0)return 0;

    int frames_per_second=0;
    for (int i = 0; i < (int)IVR_Frames.size(); i++)
    {
        frames_per_second += (int)IVR_Frames[i].IVR_FrameFPS;
    }

    frames_per_second = frames_per_second / (int)IVR_Frames.size();

    return frames_per_second;
}

int CIVRVirtualCamera::IVR_GetBestFPS()
{
    //If we not have Frames to average...
    if(IVR_Frames.size()==0)return 0;

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

    for (int i = 0; i < (int)IVR_Frames.size(); i++)
    {
        fpsFloatData.push_back(IVR_Frames[i].IVR_FrameFPS);
    }

    Mat fpsData(1,IVR_Frames.size(), CV_32F, &fpsFloatData[0]);
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

    //The index stores the FPS :)
    return fpsOccurenciesData[0].index;
}

int CIVRVirtualCamera::IVR_GetDesiredFPS()
{
    //If we not have Frames to average...
    if(IVR_Frames.size()==0)return 0;
    return IVR_DesiredFPS;
}

int CIVRVirtualCamera::IVR_GetFPSByTimeApprox()
{
    int FPS     = 60;
    int nFrames = 1;
    int milisec = 1.000;
    //If we not have Frames to average...
    if(IVR_Frames.size()==0)return 0;

    QDateTime pIVR_TakeInitialTime = QDateTime::fromMSecsSinceEpoch(IVR_Frames[0].IVR_Timestamp);
    QDateTime pIVR_TakeFinalTime   = QDateTime::fromMSecsSinceEpoch(IVR_Frames[IVR_Frames.size()-1].IVR_Timestamp);
    quint64   pIVR_TakeElapsedTime = pIVR_TakeInitialTime.msecsTo(pIVR_TakeFinalTime);

    FPS     = IVR_GetBestFPS();
    nFrames = IVR_Frames.size();

    //This is the system actual resultant video Time(How much longer it play)
    quint64 actualElapsed = ((nFrames / FPS) * milisec);

    //If the system will create a faster video, we try approximate the FPS to the real time-elapsed
    if(actualElapsed < pIVR_TakeElapsedTime)
    {
        while(actualElapsed < pIVR_TakeElapsedTime)
        {
            FPS--;
            actualElapsed = nFrames / FPS;
        }
    }

    return FPS;
}

bool CIVRVirtualCamera::IVR_CameraTake(QString pIVR_VideoTake)
{
    CIVRCameraTake pTake;

    //Merge the Frames with the actual video recorded.
    pTake.IVR_Frames    = IVR_Frames;
    pTake.IVR_VideoTake = pIVR_VideoTake;

    //Push a new recording take
    IVR_Takes.push_back(pTake);

    //creanup and shrink the camera recording buffer
    IVR_Frames.clear();
    return true;
}
