/************************************************************************/
/*Project              :IVR CameraMan - Unreal Plugin                   */
/*Creation Date/Author :William Wolff - 02/18/2021                      */
/*                                                                      */
/*Copyright (c) 2021 William Wolff. All rights reserved                 */
/************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "IVR_DefaultCameraParameters.generated.h"

/**
* Enumerates all possible optical Flowtypes:
*
*/
UENUM(BlueprintType)
enum class EIVR_VideoResolution : uint8
{
	IVR_SD 		UMETA(DisplayName = "SD (Standard Definition)"),
	IVR_HD		UMETA(DisplayName = "HD (High Definition)"),
	IVR_FHD		UMETA(DisplayName = "Full HD (FHD)"),
	IVR_QHD		UMETA(DisplayName = "QHD (Quad HD)"),
	IVR_2K		UMETA(DisplayName = "2K video"),
	IVR_4K		UMETA(DisplayName = "4K video or Ultra HD (UHD)"),
	IVR_8K		UMETA(DisplayName = "8K video or Full Ultra HD")
};
/**
 * 
 */
UCLASS()
class IVRCAMERAMANEDITOR_API UIVR_DefaultCameraParameters : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite , DisplayName ="Recording Path", Category = "Default Parameters")
		FString                   IVR_RecordingPath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "Video Resolution", Category = "Default Parameters")
		EIVR_VideoResolution      IVR_VideoResolution;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "Clear Color", Category = "Default Parameters")
		FLinearColor              IVR_ClearColor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "Gamma", Category = "Default Parameters")
		float                     IVR_Gamma;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "Stabilization Arm Distance", Category = "Default Parameters")
		float                     IVR_StabilizationArm;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "Lag Distance", Category = "Default Parameters")
		float                     IVR_LagDistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "Camera Name", Category = "Default Parameters")
		FString                   IVR_CameraManName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "Camera Velocity", Category = "Default Parameters")
		float                     IVR_CameraVelocity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "Turn Rate", Category = "Default Parameters")
		float                     IVR_BaseTurnRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "Look Up Rate", Category = "Default Parameters")
		float                     IVR_BaseLookUpRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "Debug Rendering", Category = "Default Parameters")
		bool                      IVR_DebugRendering;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "MSecs To Wait", Category = "Default Parameters")
		int                       IVR_MSecToWait;

	//Important Method to collect the struct containers
	void*    GetStructContainer(FName pPropertyName);
	UObject* GetStructObject   (FName pPropertyName);
	
};
