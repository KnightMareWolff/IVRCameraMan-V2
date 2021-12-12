#ifndef CIVRCAMERATAKE_H
#define CIVRCAMERATAKE_H

#include "IVRLowLevelSDK_global.h"
#include "IVR_Frame.h"

class IVRLOWLEVELSDK_EXPORT CIVRCameraTake
{
public:
    CIVRCameraTake();

    //For every frame recorded in a session, we record it in a list of frames.
    vector<CIVRFrame>  IVR_Frames;
    //For every take we have a video file
    QString            IVR_VideoTake;
};

#endif // CIVRCAMERATAKE_H
