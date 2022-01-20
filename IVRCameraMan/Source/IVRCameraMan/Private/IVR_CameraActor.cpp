
/************************************************************************/
/*Project              :IVR CameraMan - Unreal Plugin                   */
/*Creation Date/Author :William Wolff - 17/09/2021                      */
/*                                                                      */
/*Copyright (c) 2021 William Wolff. All rights reserved                 */
/************************************************************************/
#include "IVR_CameraActor.h"

// Sets default values
AIVR_CameraActor::AIVR_CameraActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	IVR_Root      = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = IVR_Root;

	IVR_ActorCam = CreateDefaultSubobject<UIVR_CameraComponent>(TEXT("ActorCam"));
	IVR_ActorCam->AttachToComponent(IVR_Root, FAttachmentTransformRules::KeepRelativeTransform, FName(TEXT("ActorCameraMan")));

	IVR_MoveCamera = false;
	IVR_SelfRegistered = false;

}

void AIVR_CameraActor::IVR_RegisterCamera(FString CameraName, int32 LowLevelType, int32 LowLevelRecordingMode)
{
	if (IVR_ActorCam != nullptr)
	{
		FString CamName = CameraName +	FString::FromInt(UIVR_FunctionLibrary::IVR_GetCameraCounter(LowLevelType));
		
		IVR_ActorCam->IVR_RegisterCamera(CamName, LowLevelType, LowLevelRecordingMode);
		
		IVR_SelfRegistered = true;
	}
}
// Called every frame
void AIVR_CameraActor::IVR_CustomTick()
{
	// Update target
	if ((IVR_ActorCam != nullptr) && (IVR_MoveCamera))
	{
		IVR_ActorCam->IVR_CustomTick();
	}
}


// Called when the game starts or when spawned
void AIVR_CameraActor::IVR_StartRecord()
{
	if (!IVR_MoveCamera)
	{
		if(IVR_ActorCam->IVR_StartRecord())IVR_MoveCamera = true;
	}
}

// Called when the game starts or when spawned
void AIVR_CameraActor::IVR_StopRecord(bool IVR_AutoRecord)
{
	if (IVR_MoveCamera)
	{
		if (IVR_ActorCam->IVR_StopRecord())
		{
			IVR_MoveCamera = false;
			if (IVR_AutoRecord)IVR_ActorCam->IVR_CompileVideo();
		}
		
	}
}

// Called when the game starts or when spawned
bool AIVR_CameraActor::IVR_SetTransform(FTransform pTransform)
{
	SetActorRelativeTransform(pTransform);
	IVR_ActorCam->IVR_SetTransform(pTransform);
	return true;
}