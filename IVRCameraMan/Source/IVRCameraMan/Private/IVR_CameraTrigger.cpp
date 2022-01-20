
/************************************************************************/
/*Project              :IVR CameraMan - Unreal Plugin                   */
/*Creation Date/Author :William Wolff - 02/18/2021                      */
/*                                                                      */
/*Copyright (c) 2021 William Wolff. All rights reserved                 */
/************************************************************************/
#include "IVR_CameraTrigger.h"
//Declaring Here to avoid circular declaration...
#include "IVR_OnCraneCamera.h"
#include "IVR_OnRailCamera.h"

// Sets default values
AIVR_CameraTrigger::AIVR_CameraTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Add static mesh component to actor
	IVR_TriggerVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerVolume"));
	if (!ensure(IVR_TriggerVolume != nullptr))return;

	RootComponent = IVR_TriggerVolume;

	IVR_TriggerVolume->BodyInstance.SetCollisionProfileName("Trigger");

	IVR_TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &AIVR_CameraTrigger::OnOverlapBegin);
	IVR_TriggerVolume->OnComponentEndOverlap.AddDynamic(this, &AIVR_CameraTrigger::OnOverlapEnd);

    IVR_Overlapping = false;

}

// Called when the game starts or when spawned
void AIVR_CameraTrigger::BeginPlay()
{
	Super::BeginPlay();

	//Draw Debbug box, only if enabled by the Director
	if (UIVR_FunctionLibrary::pIVR_DrawDebbug)DrawDebugBox(GetWorld(), GetActorLocation(), GetComponentsBoundingBox().GetExtent(), FColor::Turquoise, true, -1, 0, 5);
	
}

// Called every frame
void AIVR_CameraTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AIVR_CameraTrigger::OnOverlapBegin(class UPrimitiveComponent* newComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
    if (IVR_TargetActorIn == nullptr)return;

    if (IVR_Overlapping)return;

    // check if Actors do not equal nullptr and that 
    if (OtherActor && (OtherActor != this) && OtherActor->GetClass() == AIVR_CameraActor::StaticClass())
    {
        UE_LOG(LogTemp, Warning, TEXT("IVR TriggerBox Activated"));
        
        IVR_Overlapping = true;

        switch (IVR_TriggerActionIn)
        {
        case EActionType::Stimulate:
        {
            switch (IVR_InType)
            {
            case ECameraType::CameraActor:
            {
                AIVR_CameraActor* pActorCam = (AIVR_CameraActor*)IVR_TargetActorIn;
                //If the system Cannot Cast, a wrong type was informed...
                if (pActorCam->GetClass() == AIVR_CameraActor::StaticClass() ||
                    pActorCam->GetClass()->GetSuperClass() == AIVR_CameraActor::StaticClass())
                {
                    AsyncTask(ENamedThreads::GameThread, [this,pActorCam]()
                    {
                        if (pActorCam && (pActorCam->IVR_ActorCam->IVR_LowLevelType == IVR_CamType_OnActor))pActorCam->IVR_StartRecord();
                    });
                }
            }break;
            case ECameraType::OnRail:
            {
                AIVR_OnRailCamera* pRailCam = (AIVR_OnRailCamera*)IVR_TargetActorIn;

                if (pRailCam->GetClass() == AIVR_OnRailCamera::StaticClass() ||
                    pRailCam->GetClass()->GetSuperClass() == AIVR_OnRailCamera::StaticClass())
                {
                    //If the system Cannot Cast, a wrong type was informed...
                    AsyncTask(ENamedThreads::GameThread, [this,pRailCam]()
                    {
                        if (pRailCam && (pRailCam->IVR_RailCam->IVR_ActorCam->IVR_LowLevelType == IVR_CamType_OnRail))pRailCam->IVR_StartRail();
                    });
                }
            }break;
            case ECameraType::OnCrane:
            {
                AIVR_OnCraneCamera* pCraneCam = (AIVR_OnCraneCamera*)IVR_TargetActorIn;
                //If the system Cannot Cast, a wrong type was informed...
                if (pCraneCam->GetClass() == AIVR_OnCraneCamera::StaticClass() ||
                    pCraneCam->GetClass()->GetSuperClass() == AIVR_OnCraneCamera::StaticClass())
                {
                    AsyncTask(ENamedThreads::GameThread, [this,pCraneCam]()
                    {
                        if (pCraneCam && (pCraneCam->IVR_CraneCam->IVR_ActorCam->IVR_LowLevelType == IVR_CamType_OnCrane))pCraneCam->IVR_StartCrane();
                    });
                }
            }break;
            case ECameraType::Watching:
            {
                //Just Ignore For Now... :)
            }break;
            case ECameraType::None:
            {
                //Just Ignore... :)
            }break;
            }

        }break;
        case EActionType::Block:
        {
            switch (IVR_InType)
            {
            case ECameraType::CameraActor:
            {
                AIVR_CameraActor* pActorCam = (AIVR_CameraActor*)IVR_TargetActorIn;
                //If the system Cannot Cast, a wrong type was informed...
                if (pActorCam->GetClass() == AIVR_CameraActor::StaticClass() ||
                    pActorCam->GetClass()->GetSuperClass() == AIVR_CameraActor::StaticClass())
                {
                    AsyncTask(ENamedThreads::GameThread, [this,pActorCam]()
                    {
                        if (pActorCam && (pActorCam->IVR_ActorCam->IVR_LowLevelType == IVR_CamType_OnActor))pActorCam->IVR_StopRecord();
                    });
                }

            }break;
            case ECameraType::OnRail:
            {
                AIVR_OnRailCamera* pRailCam = (AIVR_OnRailCamera*)IVR_TargetActorIn;
                //If the system Cannot Cast, a wrong type was informed...
                if (pRailCam->GetClass() == AIVR_OnRailCamera::StaticClass() ||
                    pRailCam->GetClass()->GetSuperClass() == AIVR_OnRailCamera::StaticClass())
                {
                    AsyncTask(ENamedThreads::GameThread, [this, pRailCam]()
                    {
                        if (pRailCam && (pRailCam->IVR_RailCam->IVR_ActorCam->IVR_LowLevelType == IVR_CamType_OnRail))pRailCam->IVR_StopRail();
                    });
                }

            }break;
            case ECameraType::OnCrane:
            {
                AIVR_OnCraneCamera* pCraneCam = (AIVR_OnCraneCamera*)IVR_TargetActorIn;
                //If the system Cannot Cast, a wrong type was informed...
                if (pCraneCam->GetClass() == AIVR_OnCraneCamera::StaticClass() ||
                    pCraneCam->GetClass()->GetSuperClass() == AIVR_OnCraneCamera::StaticClass())
                {
                    AsyncTask(ENamedThreads::GameThread, [this,pCraneCam]()
                    {
                        if (pCraneCam && (pCraneCam->IVR_CraneCam->IVR_ActorCam->IVR_LowLevelType == IVR_CamType_OnCrane))pCraneCam->IVR_StopCrane();
                    });
                }


            }break;
            case ECameraType::Watching:
            {
                //Just Ignore For Now... :)
            }break;
            case ECameraType::None:
            {
                //Just Ignore... :)
            }break;
            }
        }break;
        }
    }
}

void AIVR_CameraTrigger::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    //Warning! This must happen before the object comparison to avoid stuck the trigger
    IVR_Overlapping = false;

    //If we not have a target object...
    if (IVR_TargetActorOut == nullptr)return;

	if (OtherActor && (OtherActor != this) && OtherActor->GetClass() == AIVR_CameraActor::StaticClass())
    {
        UE_LOG(LogTemp, Warning, TEXT("IVR TriggerBox De-Activated"));

        switch (IVR_TriggerActionOut)
        {
        case EActionType::Stimulate:
        {
            switch (IVR_OutType)
            {
            case ECameraType::CameraActor:
            {
                AIVR_CameraActor* pActorCam = (AIVR_CameraActor*)IVR_TargetActorOut;
                //If the system Cannot Cast, a wrong type was informed...
                if (pActorCam->GetClass() == AIVR_CameraActor::StaticClass() ||
                    pActorCam->GetClass()->GetSuperClass() == AIVR_CameraActor::StaticClass())
                {
                    AsyncTask(ENamedThreads::GameThread, [this,pActorCam]()
                    {
                        if (pActorCam && (pActorCam->IVR_ActorCam->IVR_LowLevelType == IVR_CamType_OnActor))pActorCam->IVR_StartRecord();
                    });
                }

            }break;
            case ECameraType::OnRail:
            {
                AIVR_OnRailCamera* pRailCam = (AIVR_OnRailCamera*)IVR_TargetActorOut;
                //If the system Cannot Cast, a wrong type was informed...
                if (pRailCam->GetClass() == AIVR_OnRailCamera::StaticClass() ||
                    pRailCam->GetClass()->GetSuperClass() == AIVR_OnRailCamera::StaticClass())
                {
                    AsyncTask(ENamedThreads::GameThread, [this,pRailCam]()
                    {
                        if (pRailCam && (pRailCam->IVR_RailCam->IVR_ActorCam->IVR_LowLevelType == IVR_CamType_OnRail))pRailCam->IVR_StartRail();
                    });
                }

            }break;
            case ECameraType::OnCrane:
            {
                AIVR_OnCraneCamera* pCraneCam = (AIVR_OnCraneCamera*)IVR_TargetActorOut;
                //If the system Cannot Cast, a wrong type was informed...
                if (pCraneCam->GetClass() == AIVR_OnCraneCamera::StaticClass() ||
                    pCraneCam->GetClass()->GetSuperClass() == AIVR_OnCraneCamera::StaticClass())
                {
                    AsyncTask(ENamedThreads::GameThread, [this,pCraneCam]()
                    {
                        if (pCraneCam && (pCraneCam->IVR_CraneCam->IVR_ActorCam->IVR_LowLevelType == IVR_CamType_OnCrane))pCraneCam->IVR_StartCrane();
                    });
                }

            }break;
            case ECameraType::Watching:
            {
                //Just Ignore For Now... :)
            }break;
            case ECameraType::None:
            {
                //Just Ignore... :)
            }break;
            }

        }break;
        case EActionType::Block:
        {
            switch (IVR_OutType)
            {
            case ECameraType::CameraActor:
            {
                AIVR_CameraActor* pActorCam = (AIVR_CameraActor*)IVR_TargetActorOut;
                //If the system Cannot Cast, a wrong type was informed...
                if (pActorCam->GetClass() == AIVR_CameraActor::StaticClass() ||
                    pActorCam->GetClass()->GetSuperClass() == AIVR_CameraActor::StaticClass())
                {
                    AsyncTask(ENamedThreads::GameThread, [this,pActorCam]()
                    {
                        if (pActorCam && (pActorCam->IVR_ActorCam->IVR_LowLevelType == IVR_CamType_OnActor))pActorCam->IVR_StopRecord();
                    });
                }

            }break;
            case ECameraType::OnRail:
            {
                AIVR_OnRailCamera* pRailCam = (AIVR_OnRailCamera*)IVR_TargetActorOut;
                //If the system Cannot Cast, a wrong type was informed...
                if (pRailCam->GetClass() == AIVR_OnRailCamera::StaticClass() ||
                    pRailCam->GetClass()->GetSuperClass() == AIVR_OnRailCamera::StaticClass())
                {
                    AsyncTask(ENamedThreads::GameThread, [this, pRailCam]()
                    {
                        if (pRailCam && (pRailCam->IVR_RailCam->IVR_ActorCam->IVR_LowLevelType == IVR_CamType_OnRail))pRailCam->IVR_StopRail();
                    });
                }

            }break;
            case ECameraType::OnCrane:
            {
                AIVR_OnCraneCamera* pCraneCam = (AIVR_OnCraneCamera*)IVR_TargetActorOut;
                //If the system Cannot Cast, a wrong type was informed...
                if (pCraneCam->GetClass() == AIVR_OnCraneCamera::StaticClass() ||
                    pCraneCam->GetClass()->GetSuperClass() == AIVR_OnCraneCamera::StaticClass())
                {
                    AsyncTask(ENamedThreads::GameThread, [this,pCraneCam]()
                    {
                        if (pCraneCam && (pCraneCam->IVR_CraneCam->IVR_ActorCam->IVR_LowLevelType == IVR_CamType_OnCrane))pCraneCam->IVR_StopCrane();
                    });
                }

            }break;
            case ECameraType::Watching:
            {
                //Just Ignore For Now... :)
            }break;
            case ECameraType::None:
            {
                //Just Ignore... :)
            }break;
            }
        }break;
        }
    }
}

