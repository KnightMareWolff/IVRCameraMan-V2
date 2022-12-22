/************************************************************************/
/*Project              :IVR CameraMan - Unreal Plugin                   */
/*Creation Date/Author :William Wolff - 02/18/2021                      */
/*                                                                      */
/*Copyright (c) 2021 William Wolff. All rights reserved                 */
/************************************************************************/
#pragma once

#include "Engine.h"
#include "GameFramework/Character.h"
#include "IVR_CameraActor.h"

//Macros Definitions
#include "IVR_CameraMan.generated.h"

UCLASS()
class IVRCAMERAMAN_API AIVR_CameraMan : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AIVR_CameraMan();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveForward (float Val );
	void MoveRight   (float Val );
	void TurnAtRate  (float Rate);
	void LookUpAtRate(float Rate);
	void StartRecord (          );
	void StopRecord  (          );
	
	//Load Default Values from .ini
	FConfigFile IVR_ProjectConfigFile;
	bool IVR_LoadDefault();
	

	UFUNCTION(Category = "CameraMan|Video|Functions", BlueprintCallable)
	void IVR_GrabbSpawner();
	UFUNCTION(Category = "CameraMan|Video|Functions", BlueprintCallable)
	void IVR_StartMan();
	UFUNCTION(Category = "CameraMan|Video|Functions", BlueprintCallable)
	void IVR_StopMan();

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = "true"), Category = "Camera Man Parameters")
	float                     IVR_CameraVelocity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Man Parameters")
	float                     IVR_BaseTurnRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Man Parameters")
	float                     IVR_BaseLookUpRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Man Parameters")
	bool                      IVR_AutoStart;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Man Parameters")
	bool                      IVR_AutoRecord;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Man Parameters")
	ERecordingMode            IVR_RecordingMode;
	UPROPERTY(EditAnywhere, Category = "Camera Man Parameters")
	bool  IVR_UseEffects;
	UPROPERTY(EditAnywhere, Category = "Camera Man Parameters")
    FString  IVR_EffectName;
	UPROPERTY(EditAnywhere, Category = "Camera Man Parameters")
	bool     IVR_UseCompression;
	
	//If true this camera record Stabilized Frames on the HD.(Used To know the correct files to load).
	TArray<AIVR_CameraMan*>    IVR_ChildCams;

	USceneComponent     * IVR_Root;
	UCameraComponent    * IVR_Camera;
	AIVR_CameraActor    * IVR_CharacterCam;
	

};
