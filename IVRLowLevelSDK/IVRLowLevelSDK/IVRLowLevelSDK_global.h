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

//The render buffer structure will be the main way to exchange data between the dll and Unreal
typedef class IVRLOWLEVELSDK_EXPORT IVR_RenderBuffer
{
public:
    IVR_RenderBuffer(                               ) { IVR_IsValid=false; }
    IVR_RenderBuffer( const IVR_RenderBuffer & copy )
    {
        IVR_IsValid        = copy.IVR_IsValid;
        IVR_CameraName     = copy.IVR_CameraName;
        IVR_CameraType     = copy.IVR_CameraType;
        IVR_Width          = copy.IVR_Width;
        IVR_Height         = copy.IVR_Height;
        IVR_ColorChannels  = copy.IVR_ColorChannels;
        IVR_CameraID       = copy.IVR_CameraID;
        IVR_FrameID        = copy.IVR_FrameID;
        IVR_FrameFPS       = copy.IVR_FrameFPS;
        IVR_Position       = copy.IVR_Position;
        IVR_Rotation       = copy.IVR_Rotation;
        IVR_Scale          = copy.IVR_Scale;
        IVR_Timestamp      = copy.IVR_Timestamp;
        IVR_Buffer         = copy.IVR_Buffer;
    }

    IVR_RenderBuffer& operator = (const IVR_RenderBuffer &t)
    {
        IVR_IsValid        = t.IVR_IsValid;
        IVR_CameraName     = t.IVR_CameraName;
        IVR_CameraType     = t.IVR_CameraType;
        IVR_Width          = t.IVR_Width;
        IVR_Height         = t.IVR_Height;
        IVR_ColorChannels  = t.IVR_ColorChannels;
        IVR_CameraID       = t.IVR_CameraID;
        IVR_FrameID        = t.IVR_FrameID;
        IVR_FrameFPS       = t.IVR_FrameFPS;
        IVR_FrameDT        = t.IVR_FrameDT;
        IVR_Position       = t.IVR_Position;
        IVR_Rotation       = t.IVR_Rotation;
        IVR_Scale          = t.IVR_Scale;
        IVR_Timestamp      = t.IVR_Timestamp;
        IVR_Buffer         = t.IVR_Buffer;
        return *this;
    }

    Mat toMat()
    {
        return Mat(IVR_Height , IVR_Width, CV_8UC4 , IVR_Buffer.data());
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
    int        IVR_Width        ;//Width of the Image
    int        IVR_Height       ;//Height of the Image
    int        IVR_ColorChannels;//Collor Channels of the Image collected
    int        IVR_CameraID     ;//Camera Low Level Index
    int        IVR_FrameID      ;//Frame  Low Level index (filled when Frame was recorded into Virtual Cam)
    float      IVR_FrameFPS     ;//Frame FPS
    float      IVR_FrameDT      ;//Frame Delta Time
    QVector3D  IVR_Position     ;//Position of the Camera
    QVector3D  IVR_Rotation     ;//Rotation of the Camera
    QVector3D  IVR_Scale        ;//Scale of the Camera
    qint64     IVR_Timestamp    ;//Timestamp of the frame collecting
    QByteArray IVR_Buffer       ;//Image of the rendered buffer in Unreal
}IVR_RenderBuffer;

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
