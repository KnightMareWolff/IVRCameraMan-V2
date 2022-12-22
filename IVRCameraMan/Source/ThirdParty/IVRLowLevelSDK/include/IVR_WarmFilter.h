#ifndef CIVRWARMFILTER_H
#define CIVRWARMFILTER_H

#include "IVR_ImageFilter.h"

class CIVRWarmFilter : public CIVRImageFilter
{
public:
    CIVRWarmFilter();
    Mat IVR_ApplyFilter(const Mat pInputImage);
};

#endif // CIVRWARMFILTER_H
