#ifndef CIVRCLASSIFIERFEATURE_H
#define CIVRCLASSIFIERFEATURE_H

#include "IVRLowLevelSDK_global.h"

/**
* Represent an Feature of a Problem independent of what this problem is on the global logic system.
* This class are used for now to store the characteristics of a feature extracted from Images, but
* can be used in future to store all other types of values and characteristics for other problems.
* The main reason to make this , is about to make possible we "merge" Unreal/Game concepts with Machine
* Learning concepts! In other words we can have many "In-game" features being spawned between the game logic
* and be used during the game play to make possible the system "learn" about the User actions and things
* like that... :)
* The second most important reason is about becouse is a AActor class, we can spawn it on the system and
* All the necessary functionalities, like garbage collection, memory management , etc..are already avaliable
* to use with literally no code to be made to put in action the learning procedures, except the actions itself
* For example, since we are using actors, all of them will be spawned when we process the Image directories
* the features will be extracted and the training will happen using just some Blueprint boxes, without need
* develop an specific tool for the Training process.
*/
class IVRLOWLEVELSDK_EXPORT CIVRClassifierFeature
{
public:
    CIVRClassifierFeature();

    //Store The Aplication ID For this feature...It´s Useful when we need Store a single feature database and link with many classes.
    int IVR_FeatureID;

    //Store the Feature description...(Ex:. "This feature stores the distance between this document with the base system document" )
    QString IVR_FeatureDescription;

    //Store the Feture Value...
    float   IVR_FeatureValue;
};

#endif // CIVRCLASSIFIERFEATURE_H
