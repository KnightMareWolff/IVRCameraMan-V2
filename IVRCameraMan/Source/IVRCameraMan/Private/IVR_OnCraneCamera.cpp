
/************************************************************************/
/*Project              :IVR CameraMan - Unreal Plugin                   */
/*Creation Date/Author :William Wolff - 17/09/2021                      */
/*                                                                      */
/*Copyright (c) 2021 William Wolff. All rights reserved                 */
/************************************************************************/
#include "IVR_OnCraneCamera.h"

// Sets default values
AIVR_OnCraneCamera::AIVR_OnCraneCamera(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Set this Actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	IVR_CraneCam = nullptr;

	// Add static mesh component to actor
	IVR_CraneAnimation = CreateDefaultSubobject<UIVR_AnimationComponent>(TEXT("Crane Animation"));

	IVR_StartTime         = 0;
	IVR_FullAnimationTime = 20.0f;
	IVR_NumberOfLaps      = 0;
	IVR_MaxLoopLaps       = 3;
	IVR_LockAnimation     = false;
	IVR_AutoStart         = false;
	IVR_AutoRecord        = false;

}

// Called when the game starts or when spawned
void AIVR_OnCraneCamera::BeginPlay()
{
	Super::BeginPlay();

	if (IVR_CraneCam != nullptr)
	{
		if (!IVR_CraneCam->IVR_SelfRegistered)
		{
			FString CamName = FString("DefaultOnCraneCam");
			IVR_CraneCam->IVR_RegisterCameraML(CamName, IVR_CamType_OnCrane, (int32)IVR_RecordingMode, IVR_UseEffects, IVR_EffectName, IVR_UseCompression);
		}
	}
	else
	{
		IVR_CraneCam = GetWorld()->SpawnActor<AIVR_CameraActor>(GetDefaultAttachComponent()->GetComponentLocation(), GetDefaultAttachComponent()->GetComponentRotation());
		IVR_CraneCam->AttachToComponent(GetDefaultAttachComponent(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName(TEXT("IVR_OnCraneCam")));

		IVR_CraneAnimation->IVR_GetAnimDuration(IVR_FullAnimationTime);
		IVR_CraneAnimation->IVR_SetLoop(IVR_Loop);

		//Here we not run the custom tick(Where we register the low level Camera)...so its time to initialize some important things first...
		if (!IVR_CraneCam->IVR_SelfRegistered)
		{
			FString CamName = FString("DefaultOnCraneCam");
			IVR_CraneCam->IVR_RegisterCameraML(CamName, IVR_CamType_OnCrane, (int32)IVR_RecordingMode, IVR_UseEffects, IVR_EffectName, IVR_UseCompression);
		}

		if (IVR_AutoStart)
		{
			IVR_StartCrane();
		}
	}

	
}

// Called when the game starts or when spawned
void AIVR_OnCraneCamera::IVR_StartCrane()
{
	AsyncTask(ENamedThreads::AnyBackgroundHiPriTask, [this]()
	{
		// Time when the press play
		IVR_StartTime = GetWorld()->GetTimeSeconds();
		IVR_CraneCam->IVR_StartRecord();
		IVR_CraneAnimation->IVR_PlayTimeline();
	});
}

// Called when the game starts or when spawned
void AIVR_OnCraneCamera::IVR_StopCrane()
{
	AsyncTask(ENamedThreads::AnyBackgroundHiPriTask, [this]()
	{
		IVR_CraneCam->IVR_StopRecord(IVR_AutoRecord);
	});
}

// Called when the game starts or when spawned
void AIVR_OnCraneCamera::IVR_AddAnimationFrame(float pIVR_CranePitch, float pIVR_CraneYaw, float pIVR_CraneArmLength)
{
	//We are not using splines anymore , we need a way to include the frames changing the UCurveVector realtime...
}

// Called every frame
void AIVR_OnCraneCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Update target
	if ((IVR_CraneCam != nullptr) && (IVR_CraneCam->IVR_MoveCamera))
	{

		USceneComponent* pCamMount = GetDefaultAttachComponent();

		if (IVR_FollowActor)
		{
			FRotator CameraRot = UKismetMathLibrary::FindLookAtRotation(pCamMount->GetComponentLocation(), IVR_FollowActor->GetActorLocation());
			pCamMount->SetWorldRotation(CameraRot);
		}

		IVR_CraneCam->IVR_SetTransform(pCamMount->GetComponentTransform());
		IVR_CraneCam->IVR_CustomTick();

		AsyncTask(ENamedThreads::AnyBackgroundHiPriTask, [this]()
		{
			IVR_CurrentTime = (((GetWorld()->GetTimeSeconds() - IVR_StartTime)) / IVR_FullAnimationTime);
			IVR_CraneAnimation->IVR_AnimationStep(IVR_CurrentTime);

			if (!IVR_LockAnimation)
			{
				FVector CurrentFrame = IVR_CraneAnimation->IVR_GetCurrentFrame();

				CranePitch = CurrentFrame.Z;
				CraneYaw = CurrentFrame.Y;
				CraneArmLength = CurrentFrame.X;

				// The CurrentSplineTime start at 0.0f AND GO TO 1.0f
				if (IVR_CraneAnimation->IVR_AnimationFinished())
				{
					if (IVR_Loop)
					{
						if (IVR_NumberOfLaps == IVR_MaxLoopLaps)
						{
							IVR_StopCrane();
						}
						else
						{

							IVR_NumberOfLaps++;
							IVR_StartTime = GetWorld()->GetTimeSeconds();
							IVR_CraneAnimation->IVR_PlayTimeline();

						}
					}
					else
					{
						IVR_StopCrane();
					}
				}
			}
		});
	}
}
