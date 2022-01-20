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
            IVR_CameraID          = copy.IVR_CameraID;
            IVR_FrameID           = copy.IVR_FrameID;
            IVR_TakeID            = copy.IVR_TakeID;
            IVR_FrameFPS          = copy.IVR_FrameFPS;
            IVR_FrameDT           = copy.IVR_FrameDT;
            IVR_Timestamp         = copy.IVR_Timestamp;
        }
    }

    CIVRFrame& operator = (const CIVRFrame &t)
    {
        if(t.IVR_IsValid)
        {
        IVR_IsValid        = t.IVR_IsValid;
        IVR_CameraName     = t.IVR_CameraName;
        IVR_CameraType     = t.IVR_CameraType;
        IVR_CameraID       = t.IVR_CameraID;
        IVR_FrameID        = t.IVR_FrameID;
        IVR_TakeID         = t.IVR_TakeID;
        IVR_FrameFPS       = t.IVR_FrameFPS;
        IVR_FrameDT        = t.IVR_FrameDT;
        IVR_Timestamp      = t.IVR_Timestamp;
        }
        return *this;
    }

    CIVRFrame& operator = (IVR_FrameData &t)
    {
        if(t.IVR_IsValid)
        {
        IVR_IsValid        = t.IVR_IsValid;
        IVR_CameraName     = t.IVR_CameraName;
        IVR_CameraType     = t.IVR_CameraType;
        IVR_CameraID       = t.IVR_CameraID;
        IVR_FrameID        = t.IVR_FrameID;
        IVR_TakeID         = 0;
        IVR_FrameFPS       = t.IVR_FrameFPS;
        IVR_FrameDT        = t.IVR_FrameDT;
        IVR_Timestamp      = t.IVR_Timestamp;
        }
        return *this;
    }

    CIVRFrame& operator = (const IVR_FrameData &t)
    {
        if(t.IVR_IsValid)
        {
        IVR_IsValid        = t.IVR_IsValid;
        IVR_CameraName     = t.IVR_CameraName;
        IVR_CameraType     = t.IVR_CameraType;
        IVR_CameraID       = t.IVR_CameraID;
        IVR_FrameID        = t.IVR_FrameID;
        IVR_TakeID         = 0;
        IVR_FrameFPS       = t.IVR_FrameFPS;
        IVR_FrameDT        = t.IVR_FrameDT;
        IVR_Timestamp      = t.IVR_Timestamp;
        }
        return *this;
    }

    //When created By any app the consumer must identify if its valid!
    bool       IVR_IsValid      ;
    //Store the real camera name
    QString    IVR_CameraName   ;
    //Store the real camera type
    uint       IVR_CameraType   ;
    //Store the camera id at RenderQueue
    uint       IVR_CameraID;
    //Frame  Low Level index (filled when Frame was recorded into Virtual Cam File)
    uint       IVR_FrameID;
    //Take  Low Level index (filled when Take was recorded into Virtual Cam File)
    uint       IVR_TakeID;
    //Store the Frame FPS Collected by the camera recordings
    uint       IVR_FrameFPS;
    //Store the Frame Delta Time Collected by the camera recordings
    float      IVR_FrameDT;
    //Timestamp of the Frame Creation
    qint64     IVR_Timestamp;
};

#endif // CIVRFRAME_H
