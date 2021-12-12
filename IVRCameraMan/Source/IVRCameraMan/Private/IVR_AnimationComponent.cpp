
/************************************************************************/
/*Project              :IVR CameraMan - Unreal Plugin                   */
/*Creation Date/Author :William Wolff - 17/09/2021                      */
/*                                                                      */
/*Copyright (c) 2021 William Wolff. All rights reserved                 */
/************************************************************************/
#include "IVR_AnimationComponent.h"

// Sets default values for this component's properties
UIVR_AnimationComponent::UIVR_AnimationComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
    IVR_IsFinished = false;

    static ConstructorHelpers::FObjectFinder< UCurveVector> Curve(TEXT("CurveVector'/IVRCameraMan/Assets/CraneAnimationCurve.CraneAnimationCurve'"));
    check(Curve.Succeeded());

    IVR_TimelineKeyframes = Curve.Object;

    // Add The Timeline Component
    IVR_AnimationTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Timeline Animation"));
    IVR_AnimationTimeline->SetComponentTickEnabled(false);
    //----------------------------------------
    // Functions to be tested in network yet
    // ---------------------------------------
    //this->BlueprintCreatedComponents.Add(MyTimeline); // Add to array so it gets saved
    //MyTimeline->CreationMethod = EComponentCreationMethod::UserConstructionScript; // Indicate it comes from a blueprint so it gets cleared when we rerun construction scripts
    //MyTimeline->SetNetAddressable();    // This component has a stable name that can be referenced for replication
    //TimelineDirection = FTimeline::GetTimelineDirectionEnum();
    //MyTimeline->SetDirectionPropertyName(FName("TimelineDirection"));
    //MyTimeline->SetPropertySetObject(this); // Set which object the timeline should drive properties on
    //MyTimeline->RegisterComponent();

    IVR_AnimationTimeline->SetLooping(false);
    //IVR_AnimationTimeline->SetTimelineLength(20.0f);
    IVR_AnimationTimeline->SetTimelineLengthMode(ETimelineLengthMode::TL_LastKeyFrame);
    IVR_AnimationTimeline->SetPlaybackPosition(0.0f, false);

    //Add the float curve to the timeline and connect it to your timelines's interpolation function
    onTimelineCallback.BindUFunction(this, FName{ TEXT("TimelineCallback") });
    onTimelineFinishedCallback.BindUFunction(this, FName{ TEXT("TimelineFinishedCallback") });

    IVR_AnimationTimeline->AddInterpVector(IVR_TimelineKeyframes, onTimelineCallback);
    IVR_AnimationTimeline->SetTimelineFinishedFunc(onTimelineFinishedCallback);
	// ...
}


// Called when the game starts
void UIVR_AnimationComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


void UIVR_AnimationComponent::TimelineCallback(FVector interpolatedVal)
{
    // This function is called for every tick in the timeline.
    //FString Coord = interpolatedVal.ToString();
    IVR_CurrentAnimationFrame = interpolatedVal;
    //if (GEngine)
    //    GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, *Coord, true, FVector2D(1.5, 1.5));
}

void UIVR_AnimationComponent::TimelineFinishedCallback()
{
    // This function is called when the timeline finishes playing.
    //FString Coord = "Finished the Animation";
    IVR_IsFinished = true;
    //if (GEngine)
    //    GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, *Coord, true, FVector2D(1.5, 1.5));
}

void UIVR_AnimationComponent::IVR_PlayTimeline()
{
    if (IVR_AnimationTimeline != NULL)
    {
        IVR_AnimationTimeline->PlayFromStart();
        IVR_IsFinished = false;
    }
}

void UIVR_AnimationComponent::IVR_AnimationStep(float DeltaTime)
{
    if (IVR_AnimationTimeline != NULL)
    {
        //IVR_AnimationTimeline->TickComponent(DeltaTime, ELevelTick::LEVELTICK_TimeOnly, NULL);
        float Full;
        IVR_GetAnimDuration(Full);
        IVR_AnimationTimeline->SetPlaybackPosition(DeltaTime * Full, false);
        if (DeltaTime >= 1.0f)
        {
            TimelineFinishedCallback();
        }
    }
}

void UIVR_AnimationComponent::IVR_SetLoop(bool pIVR_Loop)
{
    if (IVR_AnimationTimeline != NULL)
    {
        IVR_AnimationTimeline->SetLooping(pIVR_Loop);
    }
}

void UIVR_AnimationComponent::IVR_SetAnimDuration(float pIVR_AnimDuration)
{
    if (IVR_AnimationTimeline != NULL)
    {
        IVR_AnimationTimeline->SetTimelineLength(pIVR_AnimDuration);
    }
}

void UIVR_AnimationComponent::IVR_GetAnimDuration(float &pIVR_AnimDuration)
{
    if (IVR_AnimationTimeline != NULL)
    {
        pIVR_AnimDuration = IVR_AnimationTimeline->GetTimelineLength();
    }
}

FVector UIVR_AnimationComponent::IVR_GetCurrentFrame()
{
    return IVR_CurrentAnimationFrame;
}

bool UIVR_AnimationComponent::IVR_AnimationFinished()
{
    return IVR_IsFinished;
}
