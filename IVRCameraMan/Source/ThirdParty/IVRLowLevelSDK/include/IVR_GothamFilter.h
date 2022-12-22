#ifndef CIVRGOTHAMFILTER_H
#define CIVRGOTHAMFILTER_H

#include "IVR_ImageFilter.h"

class CIVRGothamFilter : public CIVRImageFilter
{
public:
    CIVRGothamFilter();
    Mat IVR_ApplyFilter(const Mat pInputImage);
};

#endif // CIVRGOTHAMFILTER_H
