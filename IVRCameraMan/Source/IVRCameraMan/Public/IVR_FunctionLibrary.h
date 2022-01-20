/************************************************************************/
/*Project              :IVR CameraMan - Unreal Plugin                   */
/*Creation Date/Author :William Wolff - 02/18/2021                      */
/*                                                                      */
/*Copyright (c) 2021 William Wolff. All rights reserved                 */
/************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Kismet/KismetInputLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/MessageDialog.h"
#include "Misc/DateTime.h"
#include "Containers/Queue.h"

#if WITH_OPENCV
#include "IVROpenCVHelper.h"
OPENCV_INCLUDES_START
#undef check // the check macro causes problems with opencv headers
#include "IVR_LowLevelSDK.h"
OPENCV_INCLUDES_END
#endif

//Audio
#include "Components/AudioComponent.h"
#include "AudioDecompress.h"
#include "AudioDevice.h"
#include "ActiveSound.h"
#include "Audio.h"
#include "Developer/TargetPlatform/Public/Interfaces/IAudioFormat.h"
#include "VorbisAudioInfo.h"

//Texture2D
#include "IImageWrapper.h"
#include "IImageWrapperModule.h"
#include "Engine/Texture2D.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "DDSLoader.h"


#include "IVR_FunctionLibrary.generated.h"

using namespace std;
using namespace cv;
using namespace cv::videostab;
using namespace cv::xfeatures2d;
using namespace cv::ml;


UENUM(BlueprintType)
enum class EActionType : uint8
{
	Stimulate = 0 UMETA(DisplayName = "Stimulate"),
	Block = 1 UMETA(DisplayName = "Block"),
	Focus = 2 UMETA(DisplayName = "Focus"),
	Tracking = 3 UMETA(DisplayName = "Tracking"),
	None = 4 UMETA(DisplayName = "None")
};

UENUM(BlueprintType)
enum class ECameraType : uint8
{
	CameraActor = 0 UMETA(DisplayName = "Camera Actor"),
	OnRail = 1 UMETA(DisplayName = "OnRail Camera"),
	OnCrane = 2 UMETA(DisplayName = "OnCrane Camera"),
	Watching = 3 UMETA(DisplayName = "Watching Camera"),
	None = 4 UMETA(DisplayName = "None")
};

UENUM(BlueprintType)
enum class ERecordingMode : uint8
{
	AveragedFPS      = 0   UMETA(DisplayName = "Record with Averaged FPS"),
	BestFPS          = 1      UMETA(DisplayName = "Record with FPS Histogram"),
	DesiredFPS       = 2   UMETA(DisplayName = "Record with Desired FPS"),
	TimeAproximation = 3 UMETA(DisplayName = "Record with Time Aproximation"),
	Stabilised       = 4 UMETA(DisplayName = "Stabilize the Video after Record"),
	SuperRes         = 5  UMETA(DisplayName = "SuperRes the Video after Record")
};

/**
 * 
 */
UCLASS()
class IVRCAMERAMAN_API UIVR_FunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/**
	* This is how we are informing all the cameras the state they must follow (Start Record-0 , StopRecord-1 , Iddle-2).
	*/
	UFUNCTION(Category = "CameraMan|Video|Functions", BlueprintCallable)
	static void IVR_SingleShotRecordingMessage(int pIVR_State)
	{
		pIVR_GlobalRecording = pIVR_State;
		return;
	}

	/**
	* Support Function to get the Global recordings Path.
	*/
	UFUNCTION(Category = "CameraMan|Video|Functions", BlueprintCallable)
	static FString IVR_GetAppRepository()
	{
		return pIVR_FrameFilesPath;
	}

	/**
	* Support Function to Set the Global recordings Path.
	*/
	UFUNCTION(Category = "CameraMan|Video|Functions", BlueprintCallable)
	static void IVR_SetAppRepository(FString pIVR_AppPath)
	{
		pIVR_FrameFilesPath = pIVR_AppPath;
	}

	/**
	* This function will clean up all the camera pool and prepare the scene for a new recording.
	*/
	UFUNCTION(Category = "CameraMan|Video|Functions", BlueprintCallable)
	static void IVR_ShutdownLowLevelSystem()
	{
		pIVR_LowLevelInterface->IVR_StopGUIThread();
		pIVR_LowLevelInterface->IVR_StopThreads();
		pIVR_LowLevelInterface->IVR_CleanUpRecordVSession();
		
		pIVR_LowLevelActive  = false;
		pIVR_GlobalRecording = 2;
		pIVR_GlobalReset     = false;
	}

	/**
	* This function will Initialize the LowLevel System.
	*/
	UFUNCTION(Category = "IVR_CameraMan|Settings|Functions", BlueprintCallable)
	static void IVR_InitializeLowLevelSystem()
	{
		
		const FString SessionConfigFilePath = FPaths::ConvertRelativePathToFull(FPaths::ProjectDir() / "Config" / "IVRCameraMan" / "DefaultCameraMan.ini");
		const FString SessionConfigPath     = FPaths::ConvertRelativePathToFull(FPaths::ProjectDir() / "Config" / "IVRCameraMan" );
		FString IVR_RecordingPath;
		FString IVR_VideoResolution;
		FString IVR_DebugFlag;
		FString IVR_MSecs;
		QString pPath;
		int     pMSecToWait=600;

		//Creates a new Low Level Interface
		pIVR_LowLevelInterface = new IVRLowLevelSDK();

		//Load the Default Configuration File... ;)
		if (FPlatformFileManager::Get().GetPlatformFile().FileExists(*SessionConfigFilePath))
		{
			pIVR_ProjectConfigFile.Read(SessionConfigFilePath);

			const auto SectionName = TEXT("DefaultCameraParameters");

			if (pIVR_ProjectConfigFile.GetString(SectionName, TEXT("RecordingPath"), IVR_RecordingPath) == false)
			{
				UE_LOG(LogTemp, Error, TEXT("Could not load [%s] RecordingPath"), SectionName);
			}
			else
			{
				UE_LOG(LogTemp, Log, TEXT("%s"), *(IVR_RecordingPath));
				
				IVR_SetAppRepository(IVR_RecordingPath);

				QString Root   = QString(TCHAR_TO_UTF8(*IVR_RecordingPath));
				QString Config = QString(TCHAR_TO_UTF8(*SessionConfigPath));
				
				pIVR_LowLevelInterface->IVR_SetRootPath(Root);
				pIVR_LowLevelInterface->IVR_SetConfPath(Config);
			}

			if (pIVR_ProjectConfigFile.GetString(SectionName, TEXT("VideoResolution"), IVR_VideoResolution) == false)
			{
				UE_LOG(LogTemp, Error, TEXT("Could not load [%s] VideoResolution"), SectionName);
			}
			else
			{
				UE_LOG(LogTemp, Log, TEXT("%s"), *(IVR_VideoResolution));
				
				int IVR_Width =0;
				int IVR_Height=0;
				
				if (IVR_VideoResolution == "SD (Standard Definition)") { IVR_Width = 640; IVR_Height = 480; }
				if (IVR_VideoResolution == "HD (High Definition)") { IVR_Width = 1280; IVR_Height = 720; }
				if (IVR_VideoResolution == "Full HD (FHD)") { IVR_Width = 1920; IVR_Height = 1080; }
				if (IVR_VideoResolution == "QHD (Quad HD)") { IVR_Width = 2560; IVR_Height = 1440; }
				if (IVR_VideoResolution == "2K video") { IVR_Width = 2048; IVR_Height = 1080; }
				if (IVR_VideoResolution == "4K video or Ultra HD (UHD)") { IVR_Width = 3840; IVR_Height = 2160; }
				if (IVR_VideoResolution == "8K video or Full Ultra HD") { IVR_Width = 7680; IVR_Height = 4320; }

				pIVR_LowLevelInterface->IVR_SetResolution(IVR_Width, IVR_Height);
				
			}

			if (pIVR_ProjectConfigFile.GetString(SectionName, TEXT("DebugRendering"), IVR_DebugFlag) == false)
			{
				UE_LOG(LogTemp, Error, TEXT("Could not load [%s] DebbugRendering"), SectionName);
			}
			else
			{
				UE_LOG(LogTemp, Log, TEXT("%s"), *(IVR_DebugFlag));

				pIVR_DrawDebbug = IVR_DebugFlag.ToBool();
			}

			if (pIVR_ProjectConfigFile.GetString(SectionName, TEXT("MSecToWait"), IVR_MSecs) == false)
			{
				UE_LOG(LogTemp, Error, TEXT("Could not load [%s] MSecToWait"), SectionName);
			}
			else
			{
				UE_LOG(LogTemp, Log, TEXT("%s"), *(IVR_MSecs));

				pMSecToWait = atoi(TCHAR_TO_UTF8(*IVR_MSecs));
			}

		}
		
		pIVR_LowLevelInterface->IVR_FireUpGUIThread();
		pIVR_LowLevelInterface->IVR_FireUpThreads();
		pIVR_LowLevelActive = true;
		

		pIVR_LowLevelInterface->IVR_SetDebugMode(pIVR_DrawDebbug);
		pIVR_LowLevelInterface->IVR_SetWaitRecord(pMSecToWait);
		
		//pIVR_LowLevelInterface->IVR_SpawnStage();
		
	}

	/**
	* This function will Return if the LowLevel System are active or not.
	*/
	UFUNCTION(Category = "IVR_CameraMan|Settings|Functions", BlueprintCallable)
	static bool IVR_CheckLowLevel()
	{
		return pIVR_LowLevelActive;
	}

	/**
	* This function will Return if the LowLevel System are active or not.
	*/
	UFUNCTION(Category = "IVR_CameraMan|Settings|Functions", BlueprintCallable)
	static int IVR_GetCameraCounter(int pIVR_LowLevelType)
	{
		return pIVR_LowLevelInterface->IVR_GetCounter((uint)pIVR_LowLevelType);
	}
	
	//Global storage for the Video recording sessions
	static int                                     pIVR_GlobalRecording;
	static bool                                    pIVR_GlobalReset;
	static bool                                    pIVR_DrawDebbug;

	static FCriticalSection                        pIVR_CriticalSection;
	static FString                                 pIVR_FrameFilesPath;

	//Settings Attributes
	static FConfigFile                             pIVR_ProjectConfigFile;

	//Low Level SDK Attributes
	static IVRLowLevelSDK*                         pIVR_LowLevelInterface;
	static bool                                    pIVR_LowLevelActive;
	
};
