
/************************************************************************/
/*Project              :IVR CameraMan - Unreal Plugin                   */
/*Creation Date/Author :William Wolff - 02/18/2021                      */
/*                                                                      */
/*Copyright (c) 2021 William Wolff. All rights reserved                 */
/************************************************************************/
#pragma once

#include "Engine.h"
#include "GameFramework/Actor.h"
#include "IVR_FunctionLibrary.h" //The Function Library already have the OpenCV Includes
#include "IVR_CameraTrigger.generated.h"


UCLASS()
class IVRCAMERAMAN_API AIVR_CameraTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AIVR_CameraTrigger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool IVR_Overlapping;
	// Informs the TargetActor LowLevel Type
	//Warning! If it큦 not informed or Invalid, the action will be ignored!
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IVR_Camera Trigger Parameters")
	ECameraType IVR_InType;

	//specific target actor for overlap Begin
	UPROPERTY(EditAnywhere, Category = "IVR_Camera Trigger Parameters")
	class AActor* IVR_TargetActorIn;

	// Action to perform with the TargetActor when the Overlap Begins
	//Warning! If it큦 not an CameraActor , a Crane or RailCam It will be ignored!
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IVR_Camera Trigger Parameters")
	EActionType IVR_TriggerActionIn;

	// Informs the TargetActor LowLevel Type
	//Warning! If it큦 not informed or Invalid, the action will be ignored!
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IVR_Camera Trigger Parameters")
	ECameraType IVR_OutType;

	//specific target actor for overlap End
	UPROPERTY(EditAnywhere, Category = "IVR_Camera Trigger Parameters")
	class AActor* IVR_TargetActorOut;

	// Action to perform with the TargetActor when the Overlap Ends
	//Warning! If it큦 not an CameraActor , a Crane or RailCam It will be ignored!
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IVR_Camera Trigger Parameters")
	EActionType  IVR_TriggerActionOut;

private:
	UPROPERTY(VisibleAnywhere, Category = "IVR_Camera Trigger Parameters")
	class UBoxComponent* IVR_TriggerVolume;

	UFUNCTION(Category = "CameraMan|Objects|IVR_CameraTrigger", BlueprintCallable)
	void OnOverlapBegin(class UPrimitiveComponent* newComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION(Category = "CameraMan|Objects|IVR_CameraTrigger", BlueprintCallable)
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
