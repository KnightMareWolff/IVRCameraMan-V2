#ifndef CIVRDETAILFILTER_H
#define CIVRDETAILFILTER_H

#include "IVR_ImageFilter.h"

class CIVRDetailFilter : public CIVRImageFilter
{
public:
    CIVRDetailFilter();
    Mat IVR_ApplyFilter(const Mat pInputImage);

    float IVR_SigmaS;     //Range 0 - 200
    float IVR_SigmaR;     //Range 0 - 1
};

#endif // CIVRDETAILFILTER_H
