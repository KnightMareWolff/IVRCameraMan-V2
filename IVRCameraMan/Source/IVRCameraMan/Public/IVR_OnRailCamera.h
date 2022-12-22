/************************************************************************/
/*Project              :IVR CameraMan - Unreal Plugin                   */
/*Creation Date/Author :William Wolff - 17/09/2021                      */
/*                                                                      */
/*Copyright (c) 2021 William Wolff. All rights reserved                 */
/************************************************************************/
#pragma once

#include "Engine.h"
#include "CameraRig_Rail.h"
#include "IVR_CameraComponent.h"
#include "IVR_CameraActor.h"
#include "Components/SplineComponent.h"
#include "IVR_OnRailCamera.generated.h"

/**
 * 
 */
UCLASS()
class IVRCAMERAMAN_API AIVR_OnRailCamera : public ACameraRig_Rail
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AIVR_OnRailCamera(const FObjectInitializer& ObjectInitialier);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//-----------------------------------------
	//Base Camera Functionality
	//-----------------------------------------
	UFUNCTION(Category = "CameraMan|Objects|IVR_OnRailCamera", BlueprintCallable)
	void IVR_StartRail();
	UFUNCTION(Category = "CameraMan|Objects|IVR_OnRailCamera", BlueprintCallable)
	void IVR_StopRail();

	UPROPERTY(EditAnywhere, Category = "IVR_OnRail Camera Parameters")
	float IVR_TotalPathTime;
	UPROPERTY(EditAnywhere, Category = "IVR_OnRail Camera Parameters")
	bool  IVR_Loop;
	UPROPERTY(EditAnywhere, Category = "IVR_OnRail Camera Parameters")
	int   IVR_NumberOfLaps;
	UPROPERTY(EditAnywhere, Category = "IVR_OnRail Camera Parameters")
	int   IVR_MaxLoopLaps;
	UPROPERTY(EditAnywhere, Category = "IVR_OnRail Camera Parameters")
	bool  IVR_FollowRail;
	UPROPERTY(EditAnywhere, Category = "IVR_OnRail Camera Parameters")
	bool  IVR_AutoStart;
	UPROPERTY(EditAnywhere, Category = "IVR_OnRail Camera Parameters")
	bool  IVR_AutoRecord;
	UPROPERTY(EditAnywhere, Category = "IVR_OnRail Camera Parameters")
	ERecordingMode  IVR_RecordingMode;
	UPROPERTY(EditAnywhere, Category = "IVR_OnRail Camera Parameters")
	bool     IVR_UseEffects;
	UPROPERTY(EditAnywhere, Category = "IVR_OnRail Camera Parameters")
	FString  IVR_EffectName;
	UPROPERTY(EditAnywhere, Category = "IVR_OnRail Camera Parameters")
    bool     IVR_UseCompression;

	//specific target actor for overlap Begin
	UPROPERTY(EditAnywhere, Category = "IVR_OnRail Camera Parameters")
    class AActor* IVR_FollowActor;

	//-----------------------------------------------
	//Critical Sections
	//-----------------------------------------------
	FCriticalSection          IVR_TickSection;

	float      IVR_StartTime;
	float      IVR_CurrentSplineTime;
	float      IVR_SplineLength;
	FTransform IVR_ActualTransform;
	
	AIVR_CameraActor* IVR_RailCam;
	
};
