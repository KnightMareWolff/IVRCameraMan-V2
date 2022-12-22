#ifndef CIVRCLASSIFIERPROBLEM_H
#define CIVRCLASSIFIERPROBLEM_H

#include "IVRLowLevelSDK_global.h"
#include "IVR_ClassifierClass.h"

class IVRLOWLEVELSDK_EXPORT CIVRClassifierProblem
{
public:
    CIVRClassifierProblem();

    bool AddProblemClass(QString pIVR_ClassDesc);

    bool IVR_FindClass(int IVR_ClassID       ,int &IVR_ClassIndex  );
    bool IVR_Exists   (QString pIVR_ClassDesc,int &IVR_ClassID     );

    //Store All the Classes of a ML Problem
    vector<CIVRClassifierClass *> IVR_ProblemClasses;
};

#endif // CIVRCLASSIFIERPROBLEM_H
