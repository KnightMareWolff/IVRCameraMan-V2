#ifndef CIVRPENCILSKETCHFILTER_H
#define CIVRPENCILSKETCHFILTER_H

#include "IVR_ImageFilter.h"

class CIVRPencilSketchFilter : public CIVRImageFilter
{
public:
    CIVRPencilSketchFilter();
    Mat IVR_ApplyFilter(const Mat pInputImage);

    float IVR_SigmaS;     //Range 0 - 200
    float IVR_SigmaR;     //Range 0 - 1
    float IVR_ShadeFactor;//Range 0 - 0.1 (Ex:. 0.02f)
    bool  IVR_ColorOutput;//Defines if we want a color or gray sketch...default color
};

#endif // CIVRPENCILSKETCHFILTER_H
