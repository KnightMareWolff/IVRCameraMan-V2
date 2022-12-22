#ifndef CIVRDILATEFILTER_H
#define CIVRDILATEFILTER_H

#include "IVR_ImageFilter.h"

class CIVRDilateFilter : public CIVRImageFilter
{
public:
    CIVRDilateFilter();
    Mat IVR_ApplyFilter(const Mat pInputImage);

    int   IVR_KernelSize;
    int   IVR_Iteractions;
};

#endif // CIVRDILATEFILTER_H
