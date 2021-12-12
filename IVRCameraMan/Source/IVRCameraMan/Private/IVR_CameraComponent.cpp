
/************************************************************************/
/*Project              :IVR CameraMan - Unreal Plugin                   */
/*Creation Date/Author :William Wolff - 17/09/2021                      */
/*                                                                      */
/*Copyright (c) 2021 William Wolff. All rights reserved                 */
/************************************************************************/
#include "IVR_CameraComponent.h"

// Sets default values for this component's properties
UIVR_CameraComponent::UIVR_CameraComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// Add Camera Capture
	IVR_CameraCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("FrameGrabber"));

	// Add the mechanical stabilizer to actor
	IVR_Stabilizer    = CreateDefaultSubobject<USpringArmComponent>(TEXT("FrameStabilizer"));

	// Add the audio capture component
	IVR_SoundCapture  = CreateDefaultSubobject<UAudioCaptureComponent>(TEXT("AudioCaptureDevice"));

	// Add the Collision Object
	IVR_CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	
	// Initialize the default parameters
	IVR_LoadDefault();

	//Initialize the Default Processing State
	IVR_RecordingState = 2;//Initially Iddle
	IVR_FPS = 0;
	IVR_Enabled = false;
	IVR_EnableStabilization = true;
	IVR_ImageUpdateTextureRegion = nullptr;
	IVR_LowLevelType = IVR_CamType_Component;
	IVR_LowLevelRecordingType = IVR_Recording_Mode_Best;

	SetupAttachment(IVR_Stabilizer);
	//We Attach the stabilizer to the Capsule and all the cameras on the Arm.
	//IVR_Stabilizer   ->AttachToComponent(GetCapsuleComponent(), FAttachmentTransformRules::SnapToTargetIncludingScale);
	IVR_Stabilizer->bEnableCameraLag         = true;
	IVR_Stabilizer->bEnableCameraRotationLag = true;
	IVR_Stabilizer->CameraLagMaxDistance     = IVR_LagDistance;
	IVR_Stabilizer->TargetArmLength          = IVR_StabilizationArm;

	//Attach the Frame Grabber
	IVR_CameraCapture->SetupAttachment(IVR_Stabilizer);
	//IVR_CameraCapture->AttachToComponent(IVR_Stabilizer, FAttachmentTransformRules::SnapToTargetIncludingScale);
	IVR_CameraCapture->bCaptureOnMovement = true;
	//IVR_CameraCapture->SetRelativeRotation(IVR_Stabilizer->GetRelativeRotation());
	IVR_CameraCapture->SetRelativeTransform(FTransform(FRotator(0, 0, 0), FVector(0, 0, 0), FVector(1)));

	IVR_CollisionSphere->SetupAttachment(IVR_Stabilizer);
	
	IVR_LockedRendering = false;
}


// Called when the game starts
void UIVR_CameraComponent::BeginPlay()
{
	Super::BeginPlay();
	// ...
}

bool UIVR_CameraComponent::IVR_CaptureFrame(float DeltaTime)
{
	//Here you put your Thread safe code, ok?
	AsyncTask(ENamedThreads::AnyBackgroundHiPriTask, [this, DeltaTime]()
	{
		FVector pIVR_Pos = GetComponentLocation();
		FVector pIVR_Rot = GetComponentRotation().Vector();
		FVector pIVR_Scl = GetComponentScale();

		IVR_FrameBuffer.IVR_IsValid    = true;
		IVR_FrameBuffer.IVR_CameraName = QString(TCHAR_TO_UTF8(*IVR_CameraName));
		IVR_FrameBuffer.IVR_CameraType = IVR_LowLevelType;
		IVR_FrameBuffer.IVR_Width      = IVR_Width;
		IVR_FrameBuffer.IVR_Height     = IVR_Height;
		IVR_FrameBuffer.IVR_CameraID   = IVR_LowLevelIndex;
		IVR_FrameBuffer.IVR_FrameFPS   = IVR_FPS;
		IVR_FrameBuffer.IVR_FrameDT    = DeltaTime;
		IVR_FrameBuffer.IVR_Position   = QVector3D(pIVR_Pos.X, pIVR_Pos.Y, pIVR_Pos.Z);
		IVR_FrameBuffer.IVR_Rotation   = QVector3D(pIVR_Rot.X, pIVR_Rot.Y, pIVR_Rot.Z);
		IVR_FrameBuffer.IVR_Scale      = QVector3D(pIVR_Scl.X, pIVR_Scl.Y, pIVR_Scl.Z);
		IVR_FrameBuffer.IVR_Timestamp  = FDateTime::UtcNow().ToUnixTimestamp();

		IVR_LockedRendering = true;
	});
	
	//huhuuuu!
	return true;

}

// Warning Pixel format mismatch
//EPixelFormat::PF_A2B10G10R10; // back buffer
//EPixelFormat::PF_B8G8R8A8; // target
void UIVR_CameraComponent::OnBackBufferReady(SWindow& SlateWindow, const FTexture2DRHIRef& BackBuffer)
{
	if (IsInRenderingThread())
	{
		//We want just one Image since many of them are redundant
		if (IVR_LockedRendering == false)return;
		if (!IVR_RenderTarget)return;
		if (!IVR_RenderTarget->Resource)return;

		//Get the Cached Texture Info
		FRHICommandListImmediate& RHICmdList = GRHICommandList.GetImmediateCommandList();
		FRHITexture2D* CachedTexture = IVR_RenderTarget->Resource->TextureRHI->GetTexture2D();

		FRHICopyTextureInfo CopyInfo;

		//Copy the BackBuffer to the Cached Texture
		RHICmdList.CopyTexture(BackBuffer, CachedTexture, CopyInfo);

		//Get the Pixels of the Image
		TArray<FColor> RawPixels;
		GetTexturePixels(CachedTexture, RawPixels);
		AsyncTask(ENamedThreads::AnyBackgroundHiPriTask, [this, RawPixels]()
		{
			IVR_BufferSection.Lock();
			
			//If you reach this condition, means you change the video size in the middle of production.
			if (IVR_FrameBuffer.IVR_Buffer.size() != (IVR_Width * IVR_Height) * RawPixels.GetTypeSize())
			{
				IVR_FrameBuffer.IVR_Buffer.clear();
			}
						
			IVR_FrameBuffer.IVR_ColorChannels = RawPixels.GetTypeSize();
			
			//Check if we are in the beginning of a recording or can use the Cached Texture
			if (IVR_FrameBuffer.IVR_Buffer.size() == 0)IVR_FrameBuffer.IVR_Buffer = QByteArray::fromRawData((char*)RawPixels.GetData(), (IVR_Width * IVR_Height) * RawPixels.GetTypeSize());
			else
			IVR_FrameBuffer.IVR_Buffer.setRawData((char*)RawPixels.GetData(), (IVR_Width * IVR_Height) * RawPixels.GetTypeSize());

			//Insert the Raw Image at the Render queue
			UIVR_FunctionLibrary::pIVR_LowLevelInterface->IVR_RecordBuffer(IVR_FrameBuffer);

			IVR_BufferSection.Unlock();
		});
		//Lock the delegate to receive another signal
		IVR_LockedRendering = false;
	}
}


// Called every frame
void UIVR_CameraComponent::IVR_CustomTick(float DeltaTime)
{
	IVR_FPS = (1.0 / DeltaTime);

	//Draw Debbug Camera, only if enabled by the Director
	if (UIVR_FunctionLibrary::pIVR_DrawDebbug)
	{
		DrawDebugCamera(GetWorld(), IVR_Stabilizer->GetComponentLocation(), IVR_Stabilizer->GetComponentRotation(), 45.0f, 10.0f, FColor::Black, false, -1, 0);
	}


	switch (IVR_RecordingState)
	{
	case 0:	//Recording...Boa 
	{
		//Means we are starting record now, so update LowLevel and MidLevel Parameters
		//See you next frame! ;)
		if (!IVR_Enabled)
		{
			//Allocate a new cached texture and Enable the BackBuffer
			AsyncTask(ENamedThreads::GameThread, [this]()
			{
				IVR_RenderTarget = NewObject<UTextureRenderTarget2D>();
				if (IVR_RenderTarget)
				{
					IVR_RenderTarget->ClearColor = IVR_ClearColor;
					IVR_RenderTarget->TargetGamma = IVR_Gamma;
					IVR_RenderTarget->RenderTargetFormat = ETextureRenderTargetFormat::RTF_RGBA8_SRGB;
					IVR_RenderTarget->InitAutoFormat(IVR_Width, IVR_Height);
				}

				IVR_CameraCapture->TextureTarget = IVR_RenderTarget;

				//We Enable the Camera
				IVR_Enabled = true;
				// Fire Up the Render Queue
				OnBackBufferReadyToPresent = FSlateApplication::Get().GetRenderer()->OnBackBufferReadyToPresent().AddUObject(this, &UIVR_CameraComponent::OnBackBufferReady);
			});
		}

		//Capture The Frame Information and free the BackBuffer to be sent to LowLevel API.
		IVR_CaptureFrame(DeltaTime);

	}break;
	case 1: //Stop Recording
	{
		//The manager will take care to change the transition in the next checking...
		if (IVR_Enabled)
		{
			//We Disable the Camera
			IVR_Enabled = false;

			OnBackBufferReadyToPresent.Reset();

			IVR_RenderTarget = nullptr;
			IVR_CameraCapture->TextureTarget = nullptr;
		}
	}break;
	}
}

bool UIVR_CameraComponent::IVR_StartRecord()
{
	if (IVR_RecordingState == 0)
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("You Cannot Start something already running..."), true, FVector2D(1.5, 1.5));
		return false;
	}

	//If the camera are lowlevel recording (IsRecording == true) we bypass the start command.
	bool LowLevelRecordingState = UIVR_FunctionLibrary::pIVR_LowLevelInterface->IVR_CheckRecordingState((uint)IVR_LowLevelIndex);
	if (LowLevelRecordingState)return false;

	IVR_RecordingState = 0;
	return true;
}

bool UIVR_CameraComponent::IVR_StopRecord()
{
	if (IVR_RecordingState != 0)
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("You Cannot Stop something already Iddle..."), true, FVector2D(1.5, 1.5));
		return false;
	}

	IVR_RecordingState = 1;
	return true;
}
bool UIVR_CameraComponent::IVR_CompileVideo()
{
	if (IVR_RecordingState == 0)
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("Why record if the frames are being recorded yet?"), true, FVector2D(1.5, 1.5));
		return false;
	}

	//If the camera are lowlevel recording (IsRecording == true) we bypass the start command.
	bool LowLevelRecordingState = UIVR_FunctionLibrary::pIVR_LowLevelInterface->IVR_CheckRecordingState((uint)IVR_LowLevelIndex);
	if (LowLevelRecordingState)return false;

	//Attention!
	//Do not try include an AsyncTask Here! There is another thread with lock in lowlevel handling it.
	//AsyncTask(ENamedThreads::AnyBackgroundHiPriTask, [this]()
	//{
		//IVR_RecCamSection.Lock();
		UIVR_FunctionLibrary::pIVR_LowLevelInterface->IVR_HeadShotVideoRecord((uint)IVR_LowLevelIndex, (uint)IVR_LowLevelType);
		//IVR_RecCamSection.Unlock();
	//});
	return true;
	
}

// Called when the game starts or when spawned
bool UIVR_CameraComponent::IVR_LoadDefault()
{
	const FString SessionConfigFilePath = FPaths::ConvertRelativePathToFull(FPaths::ProjectDir() / "Config" / "IVRCameraMan" / "DefaultCameraMan.ini");
	FString tStr;

	//If the Configuration File Not Exists Load Fixed Parameters... ;)
	if (FPlatformFileManager::Get().GetPlatformFile().FileExists(*SessionConfigFilePath))
	{
		IVR_ProjectConfigFile.Read(SessionConfigFilePath);

		const auto SectionName = TEXT("DefaultCameraParameters");

		if (IVR_ProjectConfigFile.GetString(SectionName, TEXT("RecordingPath"), IVR_RecordingPath) == false)
		{
			UE_LOG(LogTemp, Error, TEXT("Could not load [%s] RecordingPath"), SectionName);
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("%s"), *(IVR_RecordingPath));
			UIVR_FunctionLibrary::IVR_SetAppRepository(IVR_RecordingPath);
		}

		if (IVR_ProjectConfigFile.GetString(SectionName, TEXT("VideoResolution"), tStr) == false)
		{
			UE_LOG(LogTemp, Error, TEXT("Could not load [%s] VideoResolution"), SectionName);
		}
		else
		{
			if (tStr == "SD (Standard Definition)") { IVR_Width = 640; IVR_Height = 480; }
			if (tStr == "HD (High Definition)") { IVR_Width = 1280; IVR_Height = 720; }
			if (tStr == "Full HD (FHD)") { IVR_Width = 1920; IVR_Height = 1080; }
			if (tStr == "QHD (Quad HD)") { IVR_Width = 2560; IVR_Height = 1440; }
			if (tStr == "2K video") { IVR_Width = 2048; IVR_Height = 1080; }
			if (tStr == "4K video or Ultra HD (UHD)") { IVR_Width = 3840; IVR_Height = 2160; }
			if (tStr == "8K video or Full Ultra HD") { IVR_Width = 7680; IVR_Height = 4320; }
		}

		if (IVR_ProjectConfigFile.GetString(SectionName, TEXT("Gamma"), tStr) == false)
		{
			UE_LOG(LogTemp, Error, TEXT("Could not load [%s] Gamma"), SectionName);
		}
		else
		{
			IVR_Gamma = atof(TCHAR_TO_UTF8(*tStr));
			UE_LOG(LogTemp, Log, TEXT("%s"), *(tStr));
		}

		if (IVR_ProjectConfigFile.GetString(SectionName, TEXT("StabilizationArm"), tStr) == false)
		{
			UE_LOG(LogTemp, Error, TEXT("Could not load [%s] StabilizationArm"), SectionName);
		}
		else
		{
			IVR_StabilizationArm = atof(TCHAR_TO_UTF8(*tStr));
			UE_LOG(LogTemp, Log, TEXT("%s"), *(tStr));
		}

		if (IVR_ProjectConfigFile.GetString(SectionName, TEXT("LagDistance"), tStr) == false)
		{
			UE_LOG(LogTemp, Error, TEXT("Could not load [%s] LagDistance"), SectionName);
		}
		else
		{
			IVR_LagDistance = atof(TCHAR_TO_UTF8(*tStr));
			UE_LOG(LogTemp, Log, TEXT("%s"), *(tStr));
		}

		//Set the default Camera component name
		IVR_CameraName = "DefaultCameraComponent";
		

		return true;
	}
	else
	{
		IVR_Width  = 1280;
		IVR_Height = 720;
		IVR_ClearColor = FLinearColor::Black;
		IVR_Gamma = 1.0f;
		IVR_StabilizationArm = 0.0f;
		IVR_LagDistance = 3.0f;
		//Set the default Camera component name
		IVR_CameraName = "DefaultCameraComponent";
		return true;
	}

	return false;
}

// Called when the game starts or when spawned
bool UIVR_CameraComponent::IVR_SetTransform(FTransform pTransform)
{
	IVR_Stabilizer->SetRelativeTransform(pTransform);
	return true;
}

// Called when the game starts or when spawned
bool UIVR_CameraComponent::IVR_SetRotation(FRotator    pRotation)
{
	IVR_Stabilizer->SetRelativeRotation(pRotation);
	return true;
}


void UIVR_CameraComponent::GetTexturePixels(FTexture2DRHIRef Texture, TArray<FColor>& OutPixels)
{
	struct FReadSurfaceContext
	{
		FTexture2DRHIRef Texture;
		TArray<FColor>* OutData;
		FIntRect Rect;
		FReadSurfaceDataFlags Flags;
	};

	OutPixels.Reset();
	FReadSurfaceContext ReadSurfaceContext =
	{
		Texture,
		&OutPixels,
		FIntRect(0, 0, Texture->GetSizeXY().X, Texture->GetSizeXY().Y),
		FReadSurfaceDataFlags(RCM_UNorm, CubeFace_MAX)
	};
	ENQUEUE_RENDER_COMMAND(ReadSurfaceCommand)(
		[ReadSurfaceContext](FRHICommandListImmediate& RHICmdList)
	{
		RHICmdList.ReadSurfaceData(
			ReadSurfaceContext.Texture,
			ReadSurfaceContext.Rect,
			*ReadSurfaceContext.OutData,
			ReadSurfaceContext.Flags
		);

	});

}

/**
* This function updates the Texture Regions in the Graphics Hardware.
*
* Tip for the Day:
* Instead of call it normally like has the library does, here we will call RHI Directly scheduling a Job...Like happen with Rendering ;)
*/
void  UIVR_CameraComponent::UpdateTextureRegions(FTexture2DRHIRef IVR_Texture,
	int32                   IVR_MipIndex,
	uint32                  IVR_NumRegions,
	FUpdateTextureRegion2D* IVR_Regions,
	int32                   IVR_SrcPitch,
	uint32                  IVR_SrcBpp,
	uint8* IVR_SrcData,
	bool                    IVR_FreeData)
{
	/**
	* This code works in version 4.26 without warnings.
	* in the 4.26 version we need use FRHITexture2D directly
	* the Lambda function as a parameter
	* the Mipmap checking was removed(since not exists anymore), and we are passing 0 to the MipMap Index
	* like already was made in the original UpdateTextureRegions (See the Library functions).
	* In the tests made it works fine, since we are not using this textures for rendering, so the base mip
	* is what we are looking for... ;)
	* If you need update the plugin to the 4.26 or greater use this code and comment the actual code to
	* work fine...
	*/
	if (IVR_Texture->IsValid())
	{
		struct FUpdateTextureRegionsData
		{
			FRHITexture2D* Texture2DResource;
			int32                   MipIndex;
			uint32                  NumRegions;
			FUpdateTextureRegion2D* Regions;
			int32                   SrcPitch;
			uint32                  SrcBpp;
			uint8* SrcData;
		};

		FUpdateTextureRegionsData* RegionData = new FUpdateTextureRegionsData;
		RegionData->Texture2DResource = IVR_Texture;
		RegionData->MipIndex = IVR_MipIndex;
		RegionData->NumRegions = IVR_NumRegions;
		RegionData->Regions = IVR_Regions;
		RegionData->SrcPitch = IVR_SrcPitch;
		RegionData->SrcBpp = IVR_SrcBpp;
		RegionData->SrcData = IVR_SrcData;

		ENQUEUE_RENDER_COMMAND(UpdateTextureRegionsData)(
		[RegionData, IVR_FreeData](FRHICommandListImmediate& RHICmdList)
		{
			for (uint32 RegionIndex = 0; RegionIndex < RegionData->NumRegions; ++RegionIndex)
			{
				RHIUpdateTexture2D(
					RegionData->Texture2DResource,
					RegionData->MipIndex,
					RegionData->Regions[RegionIndex],
					RegionData->SrcPitch,
					RegionData->SrcData
					+ RegionData->Regions[RegionIndex].SrcY * RegionData->SrcPitch
					+ RegionData->Regions[RegionIndex].SrcX * RegionData->SrcBpp
				);
			}
			if (IVR_FreeData)
			{
				FMemory::Free(RegionData->Regions);
				FMemory::Free(RegionData->SrcData);
			}
			delete RegionData;
		});
	}
}


