#ifndef CIVRDUOTONEFILTER_H
#define CIVRDUOTONEFILTER_H

#include "IVR_ImageFilter.h"

class CIVRDuoToneFilter : public CIVRImageFilter
{
public:
    CIVRDuoToneFilter();
    Mat IVR_ApplyFilter(const Mat pInputImage);

    int IVR_LightIntensity;

    //Tone Channels: R=0 | G=1 | B=2
    int IVR_ToneChannel01;
    int IVR_ToneChannel02;

    //ExpressionValue
    float IVR_ExpValue;
};

#endif // CIVRDUOTONEFILTER_H
