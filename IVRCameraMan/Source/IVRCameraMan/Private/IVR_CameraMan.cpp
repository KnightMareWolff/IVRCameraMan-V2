/************************************************************************/
/*Project              :IVR CameraMan - Unreal Plugin                   */
/*Creation Date/Author :William Wolff - 02/18/2021                      */
/*                                                                      */
/*Copyright (c) 2021 William Wolff. All rights reserved                 */
/************************************************************************/
#include "IVR_CameraMan.h"

// Sets default values
AIVR_CameraMan::AIVR_CameraMan()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	IVR_Root = CreateDefaultSubobject<USceneComponent>(TEXT("IVR_Root"));
	if (IVR_Root)
	{
		RootComponent = IVR_Root;
	}

	GetCapsuleComponent()->SetupAttachment(IVR_Root);

	// Add static mesh component to actor
	IVR_Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("ManCamera"));
	if (IVR_Camera)
	{
		IVR_Camera->SetupAttachment(GetCapsuleComponent());

		IVR_Camera->SetActive(true);
		IVR_Camera->bUsePawnControlRotation = true;
		//IVR_Camera->bCameraMeshHiddenInGame = true;
	}
	//Load Default Values for the CameraMan
	IVR_LoadDefault();
	//Include Here the new Loaded Parameters
	GetCharacterMovement()->MaxWalkSpeed = IVR_CameraVelocity;

	//Camera Childs will have AutoStart = true!
	IVR_AutoStart  = false;
	IVR_AutoRecord = true;
}

// Called when the game starts or when spawned
bool AIVR_CameraMan::IVR_LoadDefault()
{
	const FString SessionConfigFilePath = FPaths::ConvertRelativePathToFull(FPaths::ProjectDir() / "Config" / "IVRCameraMan" / "DefaultCameraMan.ini");
	FString tStr;

	//If the Configuration File Not Exists Load Fixed Parameters... ;)
	if (FPlatformFileManager::Get().GetPlatformFile().FileExists(*SessionConfigFilePath))
	{
		IVR_ProjectConfigFile.Read(SessionConfigFilePath);

		const auto SectionName = TEXT("DefaultCameraParameters");

		if (IVR_ProjectConfigFile.GetString(SectionName, TEXT("CameraVelocity"), tStr) == false)
		{
			UE_LOG(LogTemp, Error, TEXT("Could not load [%s] CameraVelocity"), SectionName);
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("%s"), *(tStr));
			IVR_CameraVelocity = atof(TCHAR_TO_UTF8(*tStr));
		}

		if (IVR_ProjectConfigFile.GetString(SectionName, TEXT("BaseTurnRate"), tStr) == false)
		{
			UE_LOG(LogTemp, Error, TEXT("Could not load [%s] BaseTurnRate"), SectionName);
		}
		else
		{
			IVR_BaseTurnRate = atof(TCHAR_TO_UTF8(*tStr));
		}

		if (IVR_ProjectConfigFile.GetString(SectionName, TEXT("BaseLookUpRate"), tStr) == false)
		{
			UE_LOG(LogTemp, Error, TEXT("Could not load [%s] BaseLookUpRate"), SectionName);
		}
		else
		{
			IVR_BaseLookUpRate = atof(TCHAR_TO_UTF8(*tStr));
			UE_LOG(LogTemp, Log, TEXT("%s"), *(tStr));
		}

		return true;
	}
	else
	{
		return true;
	}

	return false;
}

// Called when the game starts or when spawned
void AIVR_CameraMan::BeginPlay()
{
	Super::BeginPlay();

	//In the case of Character we do it in the Tick...
	if (IVR_CharacterCam != nullptr)
	{
		if (!IVR_CharacterCam->IVR_SelfRegistered)
		{
			FString CamName = FString("DefaultCameraMan");
			IVR_CharacterCam->IVR_RegisterCameraML(CamName, (int32)IVR_CamType_CameraMan,(int32)IVR_RecordingMode, IVR_UseEffects, IVR_EffectName, IVR_UseCompression);
		}
	}
	else
	{
		IVR_CharacterCam = GetWorld()->SpawnActor<AIVR_CameraActor>(GetDefaultAttachComponent()->GetComponentLocation(), GetDefaultAttachComponent()->GetComponentRotation());
		IVR_CharacterCam->AttachToComponent(GetDefaultAttachComponent(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName(TEXT("IVR_CharacterCam")));

		if (!IVR_CharacterCam->IVR_SelfRegistered)
		{
			FString CamName = FString("DefaultCameraMan");
			IVR_CharacterCam->IVR_RegisterCameraML(CamName, (int32)IVR_CamType_CameraMan, (int32)IVR_RecordingMode, IVR_UseEffects, IVR_EffectName, IVR_UseCompression);
		}

		IVR_CharacterCam->IVR_SetTransform(GetActorTransform());
		IVR_Camera->SetWorldTransform(GetActorTransform());
		if (IVR_AutoStart)
		{
			StartRecord();
		}
	}
}

// Called every frame
void AIVR_CameraMan::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Update target
	if (IVR_CharacterCam != nullptr && IVR_Camera!=nullptr)
	{
		
		//USceneComponent* pSceneRoot = GetDefaultAttachComponent();

		//Use this Prints!!! I spent more than 5 hours to discover the position of character not being updated...Afff...
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, IVR_Camera->GetComponentLocation().ToString(), true, FVector2D(1.5, 1.5));
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, IVR_CharacterCam->GetActorLocation().ToString(), true, FVector2D(1.5, 1.5));
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, IVR_CharacterCam->IVR_ActorCam->IVR_Stabilizer->GetComponentLocation().ToString(), true, FVector2D(1.5, 1.5));
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, GetCapsuleComponent()->GetComponentLocation().ToString(), true, FVector2D(1.5, 1.5));
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, GetActorLocation().ToString(), true, FVector2D(1.5, 1.5));
		FVector  pLocation = GetCapsuleComponent()->GetComponentLocation();
		FRotator pRotation = GetCapsuleComponent()->GetComponentRotation();

		SetActorLocation(pLocation);
		SetActorRotation(pRotation);

		FTransform pTransform = FTransform(pRotation, pLocation, FVector(1));

		IVR_CharacterCam->IVR_SetTransform(pTransform);
		IVR_Camera->SetWorldTransform(pTransform);
		IVR_CharacterCam->IVR_CustomTick();
	}
}

// Called to bind functionality to input
void AIVR_CameraMan::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveCameraManFoward"    , this, &AIVR_CameraMan::MoveForward);
	PlayerInputComponent->BindAxis("MoveCameraManRight"     , this, &AIVR_CameraMan::MoveRight);
	PlayerInputComponent->BindAxis("MoveCameraManTurnRate"  , this, &AIVR_CameraMan::TurnAtRate);
	PlayerInputComponent->BindAxis("MoveCameraManLookUpRate", this, &AIVR_CameraMan::LookUpAtRate);
	
	PlayerInputComponent->BindAction("StartRecord"  , IE_Pressed, this, &AIVR_CameraMan::StartRecord);
	PlayerInputComponent->BindAction("StopRecord"   , IE_Pressed, this, &AIVR_CameraMan::StopRecord);
	PlayerInputComponent->BindAction("GrabbCameras" , IE_Pressed, this, &AIVR_CameraMan::IVR_GrabbSpawner); 
}

// Called to bind functionality to input
void AIVR_CameraMan::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

// Called to bind functionality to input
void AIVR_CameraMan::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

// Called to bind functionality to input
void AIVR_CameraMan::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * IVR_BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

// Called to bind functionality to input
void AIVR_CameraMan::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * IVR_BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AIVR_CameraMan::StartRecord()
{
	AsyncTask(ENamedThreads::AnyBackgroundHiPriTask, [this]()
	{
		IVR_CharacterCam->IVR_StartRecord();
	});
	UAudioMixerBlueprintLibrary::StartRecordingOutput(GetWorld(), 10000);

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("Recording Start!"), true, FVector2D(1.5, 1.5));
}

void AIVR_CameraMan::StopRecord()
{
	IVR_CharacterCam->IVR_StopRecord(IVR_AutoRecord);

	//For all Childrens we stop them based on the Father(All Childrens is Autostarted)
	for (int i = 0; i < IVR_ChildCams.Num(); i++)
	{
		IVR_ChildCams[i]->IVR_CharacterCam->IVR_StopRecord(IVR_ChildCams[i]->IVR_AutoRecord);
	}

	FString FrameFilesPath = FPaths::ConvertRelativePathToFull(UIVR_FunctionLibrary::IVR_GetAppRepository());
	FString FramesPath = FrameFilesPath + "/";

	//As a Commander Camera we give a SingleShot Stop Message
	UAudioMixerBlueprintLibrary::StopRecordingOutput(GetWorld(), EAudioRecordingExportType::WavFile, IVR_CharacterCam->IVR_ActorCam->IVR_CameraName + ".wav", FramesPath);
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Recording Stop!"), true, FVector2D(1.5, 1.5));
}

// Called when the game starts or when spawned
void AIVR_CameraMan::IVR_StartMan()
{
	StartRecord();
}

// Called when the game starts or when spawned
void AIVR_CameraMan::IVR_StopMan()
{
	StopRecord();
}

void AIVR_CameraMan::IVR_GrabbSpawner()
{
	UWorld* pWorld = GetWorld();

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(pWorld, ACameraActor::StaticClass(), FoundActors);

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString("Number Of Childrens:") + FString::FromInt(FoundActors.Num()), true, FVector2D(1.5, 1.5));

	IVR_ChildCams.Empty();
	int32 Counter = 0;
	for (AActor* TActor : FoundActors)
	{
		IVR_ChildCams.Add(NewObject<AIVR_CameraMan>());
		IVR_ChildCams[Counter] = pWorld->SpawnActorAbsolute< AIVR_CameraMan >(TActor->GetActorLocation(), TActor->GetActorRotation());

		if (IVR_ChildCams[Counter] != nullptr)
		{
			//Filling The Camera Data Initialization(Just to avoid any inconsistency...) , The only information for Mngmt is the name and ID struct 
			IVR_ChildCams[Counter]->IVR_AutoStart  = true;
			IVR_ChildCams[Counter]->IVR_AutoRecord = true;

			IVR_ChildCams[Counter]->IVR_CameraVelocity = IVR_CameraVelocity;
			IVR_ChildCams[Counter]->IVR_BaseTurnRate = IVR_BaseTurnRate;
			IVR_ChildCams[Counter]->IVR_BaseLookUpRate = IVR_BaseLookUpRate;

			IVR_ChildCams[Counter]->SetActorLocation(TActor->GetActorLocation());
			IVR_ChildCams[Counter]->SetActorRotation(TActor->GetActorRotation());

			//Register the Child
			FString CamName = FString("AutoSpawned");
			IVR_ChildCams[Counter]->IVR_CharacterCam->IVR_RegisterCameraML(CamName, IVR_CamType_CameraMan, (int32)IVR_RecordingMode, IVR_UseEffects, IVR_EffectName, IVR_UseCompression);
			Counter++;
		}
	}

	//Start Father´s Recording!
	StartRecord();
}



