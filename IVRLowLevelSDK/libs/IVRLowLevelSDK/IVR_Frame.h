#ifndef CIVRFRAME_H
#define CIVRFRAME_H

#include "IVRLowLevelSDK_global.h"

class IVRLOWLEVELSDK_EXPORT CIVRFrame
{
public:
    CIVRFrame();
    CIVRFrame( const CIVRFrame & copy )
    {
        if(copy.IVR_IsValid)
        {
            IVR_IsValid           = copy.IVR_IsValid;
            IVR_CameraName        = copy.IVR_CameraName;
            IVR_CameraType        = copy.IVR_CameraType;
            IVR_Width             = copy.IVR_Width;
            IVR_Height            = copy.IVR_Height;
            IVR_ColorChannels     = copy.IVR_ColorChannels;
            IVR_CameraID          = copy.IVR_CameraID;
            IVR_FrameID           = copy.IVR_FrameID;
            IVR_FrameFPS          = copy.IVR_FrameFPS;
            IVR_FrameDT           = copy.IVR_FrameDT;
            IVR_Position          = copy.IVR_Position;
            IVR_Rotation          = copy.IVR_Rotation;
            IVR_Scale             = copy.IVR_Scale;
            IVR_Timestamp         = copy.IVR_Timestamp;
            IVR_Buffer            = copy.IVR_Buffer;
        }
    }

    CIVRFrame& operator = (const CIVRFrame &t)
    {
        if(t.IVR_IsValid)
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
        }
        return *this;
    }

    CIVRFrame& operator = (IVR_RenderBuffer &t)
    {
        if(t.IVR_IsValid)
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
        SetBuffer(t.IVR_Buffer);
        }
        return *this;
    }

    CIVRFrame& operator = (const IVR_RenderBuffer &t)
    {
        if(t.IVR_IsValid)
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
        SetBuffer(t.IVR_Buffer);
        }
        return *this;
    }

    void SetBuffer(QByteArray pIVR_Buffer)
    {
        //IVR_Buffer = Mat(IVR_Height , IVR_Width, CV_8UC4 , pIVR_Buffer.data());
        IVR_Buffer = QByteArray::fromRawData(pIVR_Buffer.data(),pIVR_Buffer.size());
    }

    //When created By any app the consumer must identify if its valid!
    bool       IVR_IsValid      ;
    //Store the real camera name
    QString    IVR_CameraName   ;
    //Store the real camera type
    uint       IVR_CameraType   ;
    //Width of the Image
    int        IVR_Width        ;
    //Height of the Image
    int        IVR_Height       ;
    //Collor Channels of the Image collected
    int        IVR_ColorChannels;
    //Store the camera id at RenderQueue
    int        IVR_CameraID;
    //Frame  Low Level index (filled when Frame was recorded into Virtual Cam)
    int        IVR_FrameID      ;
    //Store the Frame FPS Collected by the camera recordings
    float      IVR_FrameFPS;
    //Store the Frame Delta Time Collected by the camera recordings
    float      IVR_FrameDT;
    //Reference Position for the Frame.
    QVector3D  IVR_Position;
    //Reference Rotation for the Frame.
    QVector3D  IVR_Rotation;
    //Reference Scale for the Frame.
    QVector3D  IVR_Scale;
    //Timestamp of the Frame Creation
    qint64     IVR_Timestamp;
    //Image of the rendered buffer in Unreal
    QByteArray IVR_Buffer  ;

};

#endif // CIVRFRAME_H
