#ifndef CIVREMBOSSFILTER_H
#define CIVREMBOSSFILTER_H

#include "IVR_ImageFilter.h"

class CIVREmbossFilter : public CIVRImageFilter
{
public:
    CIVREmbossFilter();
    Mat IVR_ApplyFilter(const Mat pInputImage);

    int   IVR_KernelSize;
};

#endif // CIVREMBOSSFILTER_H
