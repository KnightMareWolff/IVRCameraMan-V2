#ifndef CIVRSEPIAFILTER_H
#define CIVRSEPIAFILTER_H

#include "IVR_ImageFilter.h"

class CIVRSepiaFilter : public CIVRImageFilter
{
public:
    CIVRSepiaFilter();
    Mat IVR_ApplyFilter(const Mat pInputImage);
};

#endif // CIVRSEPIAFILTER_H
