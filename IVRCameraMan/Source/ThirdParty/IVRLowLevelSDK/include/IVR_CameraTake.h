#ifndef CIVRCAMERATAKE_H
#define CIVRCAMERATAKE_H

#include "IVRLowLevelSDK_global.h"
#include "IVR_Frame.h"

class IVRLOWLEVELSDK_EXPORT CIVRCameraTake
{
public:
    CIVRCameraTake();

    CIVRCameraTake( const CIVRCameraTake & copy )
    {
        IVR_VideoTake   = copy.IVR_VideoTake;
        IVR_TakeID      = copy.IVR_TakeID;

    }

    CIVRCameraTake& operator = (const CIVRCameraTake &t)
    {
        IVR_VideoTake   = t.IVR_VideoTake;
        IVR_TakeID      = t.IVR_TakeID;
        return *this;
    }

    //For every take we have a video file
    QString            IVR_VideoTake;
    //For every take we have a video file
    QString            IVR_FrameTake;
    //ID to be used with the Name
    uint               IVR_TakeID;
};

#endif // CIVRCAMERATAKE_H
