#include "IVR_Frame.h"

CIVRFrame::CIVRFrame()
{
    IVR_IsValid           = false;
    IVR_CameraName        = "";
    IVR_CameraType        = 0;
    IVR_CameraID          = 0;
    IVR_FrameID           = 0;
    IVR_TakeID            = 0;
    IVR_FrameFPS          = 0;
    IVR_FrameDT           = 0;
    IVR_Timestamp         = 0;
}
