#ifndef CIVRGHOSTFILTER_H
#define CIVRGHOSTFILTER_H

#include "IVR_ImageFilter.h"

class CIVRGhostFilter : public CIVRImageFilter
{
public:
    CIVRGhostFilter();
    Mat IVR_ApplyFilter(const Mat pInputImage);

    int IVR_R;
    int IVR_G;
    int IVR_B;
    int IVR_Transparency;
};

#endif // CIVRGHOSTFILTER_H
