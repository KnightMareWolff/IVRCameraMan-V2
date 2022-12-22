
/************************************************************************/
/*Project              :IVR CameraMan - Unreal Plugin                   */
/*Creation Date/Author :William Wolff - 17/09/2021                      */
/*                                                                      */
/*Copyright (c) 2021 William Wolff. All rights reserved                 */
/************************************************************************/
#pragma once

#include "Engine.h"
#include "GameFramework/Actor.h"
#include "IVR_CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "IVR_CameraActor.generated.h"

UCLASS()
class IVRCAMERAMAN_API AIVR_CameraActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AIVR_CameraActor();

public:	
	
	//-----------------------------------------
	//Base Camera Functionality
	//-----------------------------------------
	UFUNCTION(Category = "CameraMan|Objects|IVR_CameraActor", BlueprintCallable)
	void IVR_StartRecord();
	UFUNCTION(Category = "CameraMan|Objects|IVR_CameraActor", BlueprintCallable)
	void IVR_StopRecord(bool IVR_AutoRecord=true);
	UFUNCTION(Category = "CameraMan|Objects|IVR_CameraActor", BlueprintCallable)
	void IVR_CustomTick();
	UFUNCTION(Category = "CameraMan|Objects|IVR_CameraActor", BlueprintCallable)
	bool IVR_SetTransform(FTransform pTransform);
	UFUNCTION(Category = "CameraMan|Objects|IVR_CameraActor", BlueprintCallable)
	void IVR_RegisterCamera(FString CameraName, ECameraType LowLevelType, ERecordingMode LowLevelRecordingMode, bool UseEffects , FString EffectName, bool UseCompression);

	void IVR_RegisterCameraML(FString CameraName, int32 LowLevelType, int32 LowLevelRecordingMode, bool UseEffects , FString EffectName , bool UseCompression);
	bool IVR_MoveCamera;
	bool IVR_SelfRegistered;

	FCriticalSection          IVR_UpdCamSection;

	UPROPERTY(VisibleAnywhere, Category = "CameraMan|Objects|IVR_CameraActor")
	UIVR_CameraComponent* IVR_ActorCam;
	UPROPERTY(VisibleAnywhere, Category = "CameraMan|Objects|IVR_CameraActor")
	USceneComponent*      IVR_Root;

};
