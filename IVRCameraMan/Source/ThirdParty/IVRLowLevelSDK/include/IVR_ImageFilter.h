#ifndef CIVRIMAGEFILTER_H
#define CIVRIMAGEFILTER_H

#include "IVRLowLevelSDK_global.h"

class IVRLOWLEVELSDK_EXPORT CIVRImageFilter : public QObject
{
public:
    CIVRImageFilter(int pIVR_FilterType);

    virtual Mat IVR_ApplyFilter(const Mat pInputImage);

    int IVR_FilterType;
};

#endif // CIVRIMAGEFILTER_H
