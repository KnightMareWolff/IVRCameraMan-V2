
/************************************************************************/
/*Project              :IVR CameraMan - Unreal Plugin                   */
/*Creation Date/Author :William Wolff - 17/09/2021                      */
/*                                                                      */
/*Copyright (c) 2021 William Wolff. All rights reserved                 */
/************************************************************************/
#pragma once

#include "Engine.h"
#include "CameraRig_Crane.h"
#include "IVR_CameraActor.h"
#include "IVR_AnimationComponent.h"
#include "Components/SplineComponent.h"

#include "IVR_OnCraneCamera.generated.h"

/**
 * 
 */
UCLASS()
class IVRCAMERAMAN_API AIVR_OnCraneCamera : public ACameraRig_Crane
{
	GENERATED_BODY()
	
public:
	// Sets default values for this character's properties
	AIVR_OnCraneCamera(const FObjectInitializer& ObjectInitialier);

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
	void IVR_StartCrane();
	UFUNCTION(Category = "CameraMan|Objects|IVR_OnRailCamera", BlueprintCallable)
	void IVR_StopCrane();
	UFUNCTION(Category = "CameraMan|Objects|IVR_OnRailCamera", BlueprintCallable)
	void IVR_AddAnimationFrame(float pIVR_CranePitch,float pIVR_CraneYaw,float pIVR_CraneArmLength);

	UPROPERTY(EditAnywhere, Category = "IVR_OnCrane Camera Parameters")
	bool  IVR_Loop;
	UPROPERTY(EditAnywhere, Category = "IVR_OnCrane Camera Parameters")
	int   IVR_NumberOfLaps;
	UPROPERTY(EditAnywhere, Category = "IVR_OnCrane Camera Parameters")
	int   IVR_MaxLoopLaps;
	UPROPERTY(EditAnywhere, Category = "IVR_OnCrane Camera Parameters")
	UIVR_AnimationComponent* IVR_CraneAnimation;
	UPROPERTY(EditAnywhere, Category = "IVR_OnCrane Camera Parameters")
    bool  IVR_LockAnimation;
	UPROPERTY(EditAnywhere, Category = "IVR_OnCrane Camera Parameters")
	bool  IVR_AutoStart;
	UPROPERTY(EditAnywhere, Category = "IVR_OnCrane Camera Parameters")
	bool  IVR_AutoRecord;
	UPROPERTY(EditAnywhere, Category = "IVR_OnCrane Camera Parameters")
	ERecordingMode  IVR_RecordingMode;
	UPROPERTY(EditAnywhere, Category = "IVR_OnCrane Camera Parameters")
	bool  IVR_UseEffects;
	UPROPERTY(EditAnywhere, Category = "IVR_OnCrane Camera Parameters")
	FString  IVR_EffectName;
	UPROPERTY(EditAnywhere, Category = "IVR_OnCrane Camera Parameters")
	bool     IVR_UseCompression;

	float IVR_StartTime;
	float IVR_CurrentTime;
	float IVR_FullAnimationTime;

	//-----------------------------------------------
	//Critical Sections
	//-----------------------------------------------
	FCriticalSection          IVR_TickSection;


	AIVR_CameraActor        * IVR_CraneCam;

	//specific target actor for overlap Begin
	UPROPERTY(EditAnywhere, Category = "IVR_OnCrane Camera Parameters")
	class AActor* IVR_FollowActor;
	

};
