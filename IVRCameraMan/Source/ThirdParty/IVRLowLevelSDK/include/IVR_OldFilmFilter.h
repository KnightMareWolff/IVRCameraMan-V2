#ifndef CIVROLDFILMFILTER_H
#define CIVROLDFILMFILTER_H

#include "IVR_ImageFilter.h"

class CIVROldFilmFilter : public CIVRImageFilter
{
public:
    CIVROldFilmFilter();
    Mat IVR_ApplyFilter(const Mat pInputImage);

    float IVR_Treshold01;
    float IVR_ExpValue;

};

#endif // CIVROLDFILMFILTER_H
