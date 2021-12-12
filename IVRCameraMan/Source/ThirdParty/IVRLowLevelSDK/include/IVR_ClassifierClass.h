#ifndef CIVRCLASSIFIERCLASS_H
#define CIVRCLASSIFIERCLASS_H

#include "IVRLowLevelSDK_global.h"
#include "IVR_ClassifierFeature.h"

class IVRLOWLEVELSDK_EXPORT CIVRClassifierClass
{
public:
    CIVRClassifierClass();

    bool AddClassFeature(int pIVR_FetureID, QString pIVR_FeatDesc, float pIVR_FeatVal);

    vector<CIVRClassifierFeature *> IVR_ClassFeatures;
    int                             IVR_ClassID;
    QString                         IVR_ClassDescription;
};

#endif // CIVRCLASSIFIERCLASS_H
