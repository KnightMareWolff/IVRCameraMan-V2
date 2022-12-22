
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
	IVR_ActorCam->IVR_LowLevelType = IVR_CamType_OnActor;
	IVR_MoveCamera = false;
	IVR_SelfRegistered = false;

}

void AIVR_CameraActor::IVR_RegisterCamera(FString CameraName, ECameraType LowLevelType, ERecordingMode LowLevelRecordingMode, bool UseEffects, FString EffectName, bool UseCompression)
{
	if (IVR_ActorCam != nullptr)
	{
		int32 LwTp = IVR_CamType_OnActor;
		int32 LwRm = IVR_Recording_Mode_Average;

		switch (LowLevelType)
		{
		case ECameraType::CameraActor: {LwTp = IVR_CamType_OnActor; }break;
		case ECameraType::OnCrane    : {LwTp = IVR_CamType_OnCrane; }break;
		case ECameraType::OnRail     : {LwTp = IVR_CamType_OnRail ; }break;
		case ECameraType::Watching   : {return;}break;
		case ECameraType::None       : {return;}break;
		default: {return; }break;
		}

		switch (LowLevelRecordingMode)
		{
		case ERecordingMode::AveragedFPS     : {LwRm = IVR_Recording_Mode_Average; }break;
		case ERecordingMode::BestFPS         : {LwRm = IVR_Recording_Mode_Best; }break;
		case ERecordingMode::TimeAproximation: {LwRm = IVR_Recording_Mode_TimeAprox; }break;
		case ERecordingMode::DesiredFPS: {return; }break;
		case ERecordingMode::Stabilised: {return; }break;
		case ERecordingMode::SuperRes  : {return; }break;
		default: {return; }break;
		}

		FString CamName = CameraName + FString::FromInt(UIVR_FunctionLibrary::IVR_GetCameraCounter(LwTp));

		IVR_ActorCam->IVR_RegisterCamera(CamName, LwTp , LwRm);

		if (UseEffects)
		{
			IVR_ActorCam->IVR_LowLevelCam->IVR_EffectsEnabled = true;
			IVR_ActorCam->IVR_LowLevelCam->IVR_EffectsName = QString(TCHAR_TO_UTF8(*EffectName));
		}
		else
		{
			//To prevent static low level objects preserve previous values
			IVR_ActorCam->IVR_LowLevelCam->IVR_EffectsEnabled = false;
			IVR_ActorCam->IVR_LowLevelCam->IVR_EffectsName = QString(" ");
		}

		if (UseCompression)
		{
			IVR_ActorCam->IVR_LowLevelCam->IVR_CompressionEnabled = true;
		}
		else
		{
			//To prevent static low level objects preserve previous values
			IVR_ActorCam->IVR_LowLevelCam->IVR_CompressionEnabled = false;
		}

		IVR_SelfRegistered = true;
	}
}

void AIVR_CameraActor::IVR_RegisterCameraML(FString CameraName, int32 LowLevelType, int32 LowLevelRecordingMode, bool UseEffects, FString EffectName , bool UseCompression)
{
	if (IVR_ActorCam != nullptr)
	{
		FString CamName = CameraName +	FString::FromInt(UIVR_FunctionLibrary::IVR_GetCameraCounter(LowLevelType));
		
		IVR_ActorCam->IVR_RegisterCamera(CamName, LowLevelType, LowLevelRecordingMode);
		
		if (UseEffects)
		{
			IVR_ActorCam->IVR_LowLevelCam->IVR_EffectsEnabled = true;
			IVR_ActorCam->IVR_LowLevelCam->IVR_EffectsName = QString(TCHAR_TO_UTF8(*EffectName));
		}
		else
		{
			//To prevent static low level objects preserve previous values
			IVR_ActorCam->IVR_LowLevelCam->IVR_EffectsEnabled = false;
			IVR_ActorCam->IVR_LowLevelCam->IVR_EffectsName = QString(" ");
		}

		if (UseCompression)
		{
			IVR_ActorCam->IVR_LowLevelCam->IVR_CompressionEnabled = true;
		}
		else
		{
			//To prevent static low level objects preserve previous values
			IVR_ActorCam->IVR_LowLevelCam->IVR_CompressionEnabled = false;
		}
		
		IVR_SelfRegistered = true;
	}
}
// Called every frame
void AIVR_CameraActor::IVR_CustomTick()
{
	// Update target
	if ((IVR_ActorCam != nullptr) && (IVR_MoveCamera))
	{
		//AsyncTask(ENamedThreads::AnyBackgroundHiPriTask, [this]()
		//{
			IVR_ActorCam->IVR_CustomTick();
		//});
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
			if (IVR_AutoRecord)
			{
				IVR_ActorCam->IVR_CompileVideo();
				//AsyncTask(ENamedThreads::AnyBackgroundHiPriTask, [this]()
				//{
				//	while (!IVR_ActorCam->IVR_LowLevelCam->IVR_IsIddle);
					//If we finish a simulation we clean up the mess... ;)
				//	UIVR_FunctionLibrary::IVR_ShutdownLowLevelSystem();
					//Map here the funxctions to return to blueprints when the recording finish...
				//});
			}
			else
			{
				//AsyncTask(ENamedThreads::AnyBackgroundHiPriTask, [this]()
				//{
				//	while (!IVR_ActorCam->IVR_LowLevelCam->IVR_IsIddle);
					//If we finish a simulation we clean up the mess... ;)
				//	UIVR_FunctionLibrary::IVR_ShutdownLowLevelSystem();
					//Map here the funxctions to return to blueprints when the recording finish...
				//});
			}
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