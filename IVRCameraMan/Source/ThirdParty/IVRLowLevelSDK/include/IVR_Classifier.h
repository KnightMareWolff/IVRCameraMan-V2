#ifndef CIVRCLASSIFIER_H
#define CIVRCLASSIFIER_H

#include "IVRLowLevelSDK_global.h"
#include "IVR_ClassifierProblem.h"

class IVRLOWLEVELSDK_EXPORT CIVRClassifier
{
public:
    CIVRClassifier();

    //Classifier Functions
    bool IVR_InitializeClassifier(int pIVR_ClassifierCategory);
    bool IVR_BuildXSpace();
    bool IVR_ComputeFeaturesNbr();
    bool IVR_Train();

    bool IVR_AddClassInstance(QString IVR_ClassDescription);
    bool IVR_AddClassFeature (int IVR_ClassID,QString IVR_FeatureDescription,float IVR_FeatureValue);

    //The main function of the class...will be used to predict the class sent in the predict buffer.
    bool IVR_Predict(float &predictedResult,const vector<CIVRClassifierFeature *> IVR_ClassFeatures);

    bool IVR_FindClass(int IVR_ClassID,int &IVR_ClassIndex);

    bool IVR_SaveModel(QString FileName,QString RootFolder);
    bool IVR_LoadModel(QString FileName,QString RootFolder);
    bool IVR_LoadModel(QString AbsolutePath);
    bool IVR_CrossValidation();

    QString IVR_ProblemName;
    int     IVR_ClassifierCategory;

private:

    //------------------------------------------------------------------------------------------------------------
    //Acctually we have just one problem to solve....Is CNH or Not?
    //------------------------------------------------------------------------------------------------------------
    CIVRClassifierProblem *IVR_Problem;

    //OpenCV Support Vector Machine Data
    Ptr<SVM> IVR_SVM;

    Mat IVR_Features;
    Mat IVR_Labels;

    Mat IVR_PredictBuffer;

    int IVR_NumberOfFeatures;

    vector<int> searchResults;
    float       hitPercentage;
};

#endif // CIVRCLASSIFIER_H
