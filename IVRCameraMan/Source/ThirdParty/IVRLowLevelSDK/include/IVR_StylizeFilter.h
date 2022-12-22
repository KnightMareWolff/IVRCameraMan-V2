#ifndef CIVRSTYLIZEFILTER_H
#define CIVRSTYLIZEFILTER_H

#include "IVR_ImageFilter.h"

class CIVRStylizeFilter : public CIVRImageFilter
{
public:
    CIVRStylizeFilter();
    Mat IVR_ApplyFilter(const Mat pInputImage);

    float IVR_SigmaS;     //Range 0 - 200
    float IVR_SigmaR;     //Range 0 - 1
};

#endif // CIVRSTYLIZEFILTER_H
