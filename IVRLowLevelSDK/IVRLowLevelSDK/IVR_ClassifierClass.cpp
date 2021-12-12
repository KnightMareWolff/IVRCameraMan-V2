#include "IVR_ClassifierClass.h"

CIVRClassifierClass::CIVRClassifierClass()
{

}

bool CIVRClassifierClass::AddClassFeature(int pIVR_FetureID, QString pIVR_FeatDesc, float pIVR_FeatVal)
{
    CIVRClassifierFeature* tFeature;

    tFeature = new CIVRClassifierFeature();

    tFeature->IVR_FeatureID          = pIVR_FetureID;
    tFeature->IVR_FeatureDescription = pIVR_FeatDesc;
    tFeature->IVR_FeatureValue       = pIVR_FeatVal;

    IVR_ClassFeatures.push_back(tFeature);

    return true;
}
