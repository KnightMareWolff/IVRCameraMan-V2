
/************************************************************************/
/*Project              :IVR CameraMan - Unreal Plugin                   */
/*Creation Date/Author :William Wolff - 17/09/2021                      */
/*                                                                      */
/*Copyright (c) 2021 William Wolff. All rights reserved                 */
/************************************************************************/
#include "IVR_OnRailCamera.h"


// Sets default values
AIVR_OnRailCamera::AIVR_OnRailCamera(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Set this Actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bLockOrientationToRail = IVR_FollowRail;

	IVR_RailCam = nullptr;

	IVR_StartTime     = 0;
	IVR_TotalPathTime = 20.0f;
	IVR_NumberOfLaps  = 0;
	IVR_MaxLoopLaps   = 3;
	IVR_AutoStart     = false;
	IVR_AutoRecord    = false;

}

// Called when the game starts or when spawned
void AIVR_OnRailCamera::BeginPlay()
{
	Super::BeginPlay();

	if (IVR_RailCam != nullptr)
	{
		//Here we not run the custom tick(Where we register the low level Camera)...so its time to initialize some important things first...
		if (!IVR_RailCam->IVR_SelfRegistered)
		{
			FString CamName = FString("DefaultOnRailCam");
			IVR_RailCam->IVR_RegisterCameraML(CamName, IVR_CamType_OnRail, (int32)IVR_RecordingMode, IVR_UseEffects, IVR_EffectName, IVR_UseCompression);
		}
	}
	else
	{
		IVR_RailCam = GetWorld()->SpawnActor<AIVR_CameraActor>(GetDefaultAttachComponent()->GetComponentLocation(), GetDefaultAttachComponent()->GetComponentRotation());
		IVR_RailCam->AttachToComponent(GetDefaultAttachComponent(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName(TEXT("IVR_OnRailCam")));

		IVR_CurrentSplineTime = 0.0f;
		IVR_StartTime = GetWorld()->GetTimeSeconds();
		IVR_SplineLength = GetRailSplineComponent()->GetSplineLength();

		//Here we not run the custom tick(Where we register the low level Camera)...so its time to initialize some important things first...
		if (!IVR_RailCam->IVR_SelfRegistered)
		{
			FString CamName = FString("DefaultOnRailCam");
			IVR_RailCam->IVR_RegisterCameraML(CamName, IVR_CamType_OnRail, (int32)IVR_RecordingMode, IVR_UseEffects, IVR_EffectName, IVR_UseCompression);
		}

		if (IVR_AutoStart)
		{
			IVR_StartRail();
		}
	}
}

// Called when the game starts or when spawned
void AIVR_OnRailCamera::IVR_StartRail()
{
	AsyncTask(ENamedThreads::AnyBackgroundHiPriTask, [this]()
	{
		// Time when the press play...
		IVR_StartTime = GetWorld()->GetTimeSeconds();
		IVR_CurrentSplineTime = 0.0f;
		IVR_SplineLength = GetRailSplineComponent()->GetSplineLength();
		bLockOrientationToRail = IVR_FollowRail;

		IVR_RailCam->IVR_StartRecord();
	});
}

// Called when the game starts or when spawned
void AIVR_OnRailCamera::IVR_StopRail()
{
	AsyncTask(ENamedThreads::AnyBackgroundHiPriTask, [this]()
	{
		IVR_RailCam->IVR_StopRecord(IVR_AutoRecord);
	});
}

// Called every frame
void AIVR_OnRailCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Update target
	if ((IVR_RailCam != nullptr) && (IVR_RailCam->IVR_MoveCamera))
	{

		USceneComponent* pCamMount = GetDefaultAttachComponent();

		if (IVR_FollowActor)
		{
			FRotator CameraRot = UKismetMathLibrary::FindLookAtRotation(pCamMount->GetComponentLocation(), IVR_FollowActor->GetActorLocation());
			pCamMount->SetWorldRotation(CameraRot);
		}
		//First we record the frame...Warning!(This is the only thing we dont include on the thread, cause Tick sync problems)
		IVR_RailCam->IVR_SetTransform(pCamMount->GetComponentTransform());
		IVR_RailCam->IVR_CustomTick();
		AsyncTask(ENamedThreads::AnyBackgroundHiPriTask, [this]()
		{
			IVR_CurrentSplineTime = (((GetWorld()->GetTimeSeconds() - IVR_StartTime)) / IVR_TotalPathTime);
			CurrentPositionOnRail = IVR_CurrentSplineTime;

			// The CurrentSplineTime start at 0.0f AND GO TO 1.0f
			if (IVR_CurrentSplineTime >= 1.0f)
			{
				if (IVR_Loop)
				{
					if (IVR_NumberOfLaps == IVR_MaxLoopLaps)
					{
						IVR_StopRail();
					}
					else
					{

						IVR_RailCam->IVR_MoveCamera = true;
						IVR_NumberOfLaps++;

						IVR_StartTime = GetWorld()->GetTimeSeconds();
						IVR_CurrentSplineTime = 0.0f;
						CurrentPositionOnRail = 0;
					}
				}
				else
				{
					IVR_StopRail();
				}
			}
		});
	}
}


