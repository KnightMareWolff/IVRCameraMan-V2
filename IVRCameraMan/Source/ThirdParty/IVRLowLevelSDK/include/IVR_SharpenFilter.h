#ifndef CIVRSHARPENFILTER_H
#define CIVRSHARPENFILTER_H

#include "IVR_ImageFilter.h"

class CIVRSharpenFilter : public CIVRImageFilter
{
public:
    CIVRSharpenFilter();
    Mat IVR_ApplyFilter(const Mat pInputImage);

    //ExpressionValue
    float IVR_ExpValue;
};

#endif // CIVRSHARPENFILTER_H
