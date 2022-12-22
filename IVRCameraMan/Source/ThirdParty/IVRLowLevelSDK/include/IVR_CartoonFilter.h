#ifndef CIVRCARTOONFILTER_H
#define CIVRCARTOONFILTER_H

#include "IVR_ImageFilter.h"

class IVRLOWLEVELSDK_EXPORT CIVRCartoonFilter : public CIVRImageFilter
{
public:
    CIVRCartoonFilter();
    Mat IVR_ApplyFilter(const Mat pInputImage);

    float IVR_Treshold01;
    float IVR_Treshold02;
    int   IVR_ApertureSize;
    bool  IVR_UseL2Gradient;
    int   IVR_KernelSize;
};

#endif // CIVRCARTOONFILTER_H
