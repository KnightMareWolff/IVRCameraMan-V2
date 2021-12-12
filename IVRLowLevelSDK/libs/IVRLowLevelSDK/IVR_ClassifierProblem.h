#ifndef CIVRCLASSIFIERPROBLEM_H
#define CIVRCLASSIFIERPROBLEM_H

#include "IVRLowLevelSDK_global.h"
#include "IVR_ClassifierClass.h"

class IVRLOWLEVELSDK_EXPORT CIVRClassifierProblem
{
public:
    CIVRClassifierProblem();

    int AddProblemClass(int pIVR_ClassID, QString pIVR_ClassDesc);

    //Store All the Classes of a ML Problem
    vector<CIVRClassifierClass *> DAI_ProblemClasses;
};

#endif // CIVRCLASSIFIERPROBLEM_H
