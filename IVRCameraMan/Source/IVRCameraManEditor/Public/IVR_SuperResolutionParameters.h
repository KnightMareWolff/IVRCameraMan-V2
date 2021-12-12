/************************************************************************/
/*Project              :IVR CameraMan - Unreal Plugin                   */
/*Creation Date/Author :William Wolff - 02/18/2021                      */
/*                                                                      */
/*Copyright (c) 2021 William Wolff. All rights reserved                 */
/************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "IVR_SuperResolutionParameters.generated.h"


/**
* Enumerates all possible optical Flowtypes:
*
*/
UENUM(BlueprintType)
enum class EIVR_OpticalFlowType : uint8
{
	IVR_Farneback		UMETA(DisplayName = "Farneback"),
	IVR_TVL1			UMETA(DisplayName = "TVL1"),
	IVR_Brox		    UMETA(DisplayName = "Brox"),
	IVR_PYRLK			UMETA(DisplayName = "PYRLK")
};

/**
 * 
 */
UCLASS()
class IVRCAMERAMANEDITOR_API UIVR_SuperResolutionParameters : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "Resolution Scale", Category = "Experimental Parameters")
	int32                     IVR_SuperScale;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "Algorithm Iterations", Category = "Experimental Parameters")
	int32                     IVR_SuperIterations;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "Temporal Area Radius", Category = "Experimental Parameters")
    int32                     IVR_SuperTemporalRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "Optical Flow Type", Category = "Experimental Parameters")
	EIVR_OpticalFlowType      IVR_SuperOpticalFlow;

	//Important Method to collect the struct containers
	void* GetStructContainer(FName pPropertyName);
	UObject* GetStructObject(FName pPropertyName);
};
