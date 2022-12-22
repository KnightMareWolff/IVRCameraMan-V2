#ifndef CIVRBINARYFILTER_H
#define CIVRBINARYFILTER_H

#include "IVR_ImageFilter.h"

class CIVRBinaryFilter : public CIVRImageFilter
{
public:
    CIVRBinaryFilter();
    Mat IVR_ApplyFilter(const Mat pInputImage);
};

#endif // CIVRBINARYFILTER_H
