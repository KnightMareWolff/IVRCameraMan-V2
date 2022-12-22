#ifndef CIVRINVERTFILTER_H
#define CIVRINVERTFILTER_H

#include "IVR_ImageFilter.h"

class CIVRInvertFilter : public CIVRImageFilter
{
public:
    CIVRInvertFilter();
    Mat IVR_ApplyFilter(const Mat pInputImage);
};

#endif // CIVRINVERTFILTER_H
