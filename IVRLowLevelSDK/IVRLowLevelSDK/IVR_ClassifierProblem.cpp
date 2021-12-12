#include "IVR_ClassifierProblem.h"

CIVRClassifierProblem::CIVRClassifierProblem()
{

}

int CIVRClassifierProblem::AddProblemClass(int pIVR_ClassID, QString pIVR_ClassDesc)
{
    CIVRClassifierClass *tClass;

    tClass = new CIVRClassifierClass();

    tClass->IVR_ClassID          = pIVR_ClassID;
    tClass->IVR_ClassDescription = pIVR_ClassDesc;

    DAI_ProblemClasses.push_back(tClass);

    return DAI_ProblemClasses.size()-1;
}
