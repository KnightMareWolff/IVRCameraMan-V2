#ifndef IVRLOWLEVELSDK_GLOBAL_H
#define IVRLOWLEVELSDK_GLOBAL_H

#include "IVR_Config.h"

#define IVR_CamType_CameraMan 0
#define IVR_CamType_Component 1
#define IVR_CamType_OnRail    2
#define IVR_CamType_OnCrane   3
#define IVR_CamType_OnActor   4

#define IVR_Recording_Mode_Average   0
#define IVR_Recording_Mode_Best      1
#define IVR_Recording_Mode_Desired   2
#define IVR_Recording_Mode_TimeAprox 3
#define IVR_Recording_Mode_Stabilise 4
#define IVR_Recording_Mode_SuperRes  5

#define IVR_Task_Type_Buffer  0
#define IVR_Task_Type_Take    1
#define IVR_Task_Type_Gbuffer 2

#define IVR_Image_Filter_Cartoon   0
#define IVR_Image_Filter_Histogram 1
#define IVR_Image_Filter_Binary    2
#define IVR_Image_Filter_Crop      3
#define IVR_Image_Filter_Contours  4
#define IVR_Image_Filter_Shapes    5
#define IVR_Image_Filter_Dilate    6
#define IVR_Image_Filter_OldFilm   7
#define IVR_Image_Filter_PencilSkt 8
#define IVR_Image_Filter_Sepia     9
#define IVR_Image_Filter_Emboss    10
#define IVR_Image_Filter_DuoTone   11
#define IVR_Image_Filter_Warm      12
#define IVR_Image_Filter_Cold      13
#define IVR_Image_Filter_Gotham    14
#define IVR_Image_Filter_Sharpen   15
#define IVR_Image_Filter_Detail    16
#define IVR_Image_Filter_Invert    17
#define IVR_Image_Filter_Stylize   18
#define IVR_Image_Filter_Ghost     19

#define IVR_Compressor_Type_Zlib       0
#define IVR_Compressor_Type_Miniz      1
#define IVR_Compressor_Type_Libdeflate 2
#define IVR_Compressor_Type_QuickLZ    3


//The render buffer structure will be the main way to exchange data between the dll and Unreal
typedef class IVRLOWLEVELSDK_EXPORT IVR_RenderBuffer
{
public:
    IVR_RenderBuffer()
    {
        IVR_Width          = 0;
        IVR_Height         = 0;
        IVR_ColorChannels  = 0;
        IVR_ShrinkSize     = 0;
        IVR_Buffer         = Mat();
    }

    IVR_RenderBuffer( const IVR_RenderBuffer & copy )
    {

        IVR_Width          = copy.IVR_Width;
        IVR_Height         = copy.IVR_Height;
        IVR_ColorChannels  = copy.IVR_ColorChannels;
        IVR_ShrinkSize     = copy.IVR_ShrinkSize;

        if (IVR_Buffer.total() > 0)IVR_Buffer = Mat();

        IVR_Buffer         = copy.IVR_Buffer.clone();
    }

    IVR_RenderBuffer& operator = (const IVR_RenderBuffer &t)
    {
        IVR_Width          = t.IVR_Width;
        IVR_Height         = t.IVR_Height;
        IVR_ColorChannels  = t.IVR_ColorChannels;
        IVR_ShrinkSize     = t.IVR_ShrinkSize;

        if (IVR_Buffer.total() > 0)IVR_Buffer = Mat();

        IVR_Buffer         = t.IVR_Buffer.clone();
        return *this;
    }

    Mat toMat()
    {
        //return Mat(IVR_Height , IVR_Width, CV_8UC4 , IVR_Buffer.data());
        return IVR_Buffer.clone();
    }

    //-----------------------------------------------------------------------
    //LowLevel Data - Used by INS to Process data in the Low Level
    //-----------------------------------------------------------------------
    int        IVR_Width        ;//Width of the Image
    int        IVR_Height       ;//Height of the Image
    int        IVR_ColorChannels;//Collor Channels of the Image collected
    ulong      IVR_ShrinkSize   ;//Size of the shrinked buffer

    Mat IVR_Buffer       ;//Image of the rendered buffer in Unreal
}IVR_RenderBuffer;

typedef class IVRLOWLEVELSDK_EXPORT IVR_FrameData
{
public:
    IVR_FrameData()
    {
        IVR_IsValid        =false;
        IVR_CameraName     = "";
        IVR_CameraType     = IVR_CamType_Component;
        IVR_CameraID       = 0;
        IVR_FrameID        = 0;
        IVR_FrameFPS       = 0;
        IVR_FrameDT        = 0;
        IVR_Timestamp      = 0;
    }

    IVR_FrameData( const IVR_FrameData & copy )
    {
        IVR_IsValid        = copy.IVR_IsValid;
        IVR_CameraName     = copy.IVR_CameraName;
        IVR_CameraType     = copy.IVR_CameraType;
        IVR_CameraID       = copy.IVR_CameraID;
        IVR_FrameID        = copy.IVR_FrameID;
        IVR_FrameFPS       = copy.IVR_FrameFPS;
        IVR_FrameDT        = copy.IVR_FrameDT;
        IVR_Timestamp      = copy.IVR_Timestamp;
    }

    IVR_FrameData& operator = (const IVR_FrameData &t)
    {
        IVR_IsValid        = t.IVR_IsValid;
        IVR_CameraName     = t.IVR_CameraName;
        IVR_CameraType     = t.IVR_CameraType;
        IVR_CameraID       = t.IVR_CameraID;
        IVR_FrameID        = t.IVR_FrameID;
        IVR_FrameFPS       = t.IVR_FrameFPS;
        IVR_FrameDT        = t.IVR_FrameDT;
        IVR_Timestamp      = t.IVR_Timestamp;

        return *this;
    }

    //-----------------------------------------------------------------------
    //Consumer Data - Used by Consumer to inform important data to Low Level
    //-----------------------------------------------------------------------
    bool       IVR_IsValid      ;//When created By any app the consumer must identify if its valid!
    QString    IVR_CameraName   ;//Store the real camera name
    uint       IVR_CameraType   ;//Store the real camera type

    //-----------------------------------------------------------------------
    //LowLevel Data - Used by INS to Process data in the Low Level
    //-----------------------------------------------------------------------
    uint       IVR_CameraID     ;//Camera Low Level Index
    uint       IVR_FrameID      ;//Frame  Low Level index (filled when Frame was recorded into Virtual Cam)
    uint       IVR_FrameFPS     ;//Frame FPS
    float      IVR_FrameDT      ;//Frame Delta Time
    qint64     IVR_Timestamp    ;//Timestamp of the frame collecting
}IVR_FrameData;


//This is the first set of features generated for the Machine Learning Process
typedef struct IVRLOWLEVELSDK_EXPORT IVR_Features
{
    int    IVR_NumberOfAreas;
    int    IVR_NumberOfInterestingAreas;
    int    IVR_NumberOfSquares;
    int    IVR_NumberOfRetangles;
    double IVR_ContourArea;
    double IVR_BiggerDimentionArea;
    double IVR_SmallerDimentionArea;
    double IVR_HistogramDifference;
    double IVR_ContourAngle;
}IVR_Features;

#endif // IVRLOWLEVELSDK_GLOBAL_H
