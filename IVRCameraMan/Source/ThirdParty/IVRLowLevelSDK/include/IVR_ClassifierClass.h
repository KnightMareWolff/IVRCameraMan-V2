#ifndef CIVRCLASSIFIERCLASS_H
#define CIVRCLASSIFIERCLASS_H

#include "IVRLowLevelSDK_global.h"
#include "IVR_ClassifierFeature.h"

class IVRLOWLEVELSDK_EXPORT CIVRClassifierClass
{
public:
    CIVRClassifierClass();

    bool AddClassFeature(QString pIVR_FeatDesc, float pIVR_FeatVal);

    bool IVR_FindFeature(int IVR_FeatureID    ,int &IVR_FeatureIndex);
    bool IVR_Exists     (QString pIVR_FeatDesc,int &IVR_FeatureID   );

    vector<CIVRClassifierFeature *> IVR_ClassFeatures;
    int                             IVR_ClassID;
    QString                         IVR_ClassDescription;
};

#endif // CIVRCLASSIFIERCLASS_H
