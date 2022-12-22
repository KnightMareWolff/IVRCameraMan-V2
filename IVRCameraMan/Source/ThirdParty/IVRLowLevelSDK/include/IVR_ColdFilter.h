#ifndef CIVRCOLDFILTER_H
#define CIVRCOLDFILTER_H

#include "IVR_ImageFilter.h"

class CIVRColdFilter : public CIVRImageFilter
{
public:
    CIVRColdFilter();
    Mat IVR_ApplyFilter(const Mat pInputImage);
};

#endif // CIVRCOLDFILTER_H
