/************************************************************************/
/*Project              :IVR CameraMan - Unreal Plugin                   */
/*Creation Date/Author :William Wolff - 02/18/2021                      */
/*                                                                      */
/*Copyright (c) 2021 William Wolff. All rights reserved                 */
/************************************************************************/
#pragma once

#include "Engine.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Kismet/KismetInputLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Misc/MessageDialog.h"
#include "Misc/DateTime.h"
#include "Containers/Queue.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

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
	Block     = 1 UMETA(DisplayName = "Block"),
	Focus     = 2 UMETA(DisplayName = "Focus"),
	Tracking  = 3 UMETA(DisplayName = "Tracking"),
	None      = 4 UMETA(DisplayName = "None")
};

/**
* Enumerates all Camera types.
*/
UENUM(BlueprintType)
enum class ECameraType : uint8
{
	CameraActor = 0 UMETA(DisplayName = "Camera Actor"),
	OnRail      = 1 UMETA(DisplayName = "OnRail Camera"),
	OnCrane     = 2 UMETA(DisplayName = "OnCrane Camera"),
	Watching    = 3 UMETA(DisplayName = "Watching Camera"),
	None        = 4 UMETA(DisplayName = "None")
};

/**
* Enumerates all recording Nodes.
*/
UENUM(BlueprintType)
enum class ERecordingMode : uint8
{
	AveragedFPS      = 0 UMETA(DisplayName = "Record with Averaged FPS"),
	BestFPS          = 1 UMETA(DisplayName = "Record with FPS Histogram"),
	DesiredFPS       = 2 UMETA(DisplayName = "Record with Desired FPS"),
	TimeAproximation = 3 UMETA(DisplayName = "Record with Time Aproximation"),
	Stabilised       = 4 UMETA(DisplayName = "Stabilize the Video after Record"),
	SuperRes         = 5 UMETA(DisplayName = "SuperRes the Video after Record")
};

/**
* Enumerates all effect types.
*/
UENUM(BlueprintType)
enum class EEffectType : uint8
{
	Cartoon      = 0  UMETA(DisplayName = "Cartoon Effect"),
	Binary       = 1  UMETA(DisplayName = "Binary Effect"),
	Dilate       = 2  UMETA(DisplayName = "Dilate Effect"),
	OldFilm      = 3  UMETA(DisplayName = "Old Film Record Effect"),
	PencilSketch = 4  UMETA(DisplayName = "Pencil Sketch Effect"),
	Sepia        = 5  UMETA(DisplayName = "Sepia Effect"),
	Emboss       = 6  UMETA(DisplayName = "Emboss Effect"),
	DuoTone      = 7  UMETA(DisplayName = "Duo Tone Effect"),
	Warm         = 8  UMETA(DisplayName = "Warm Effect"),
	Cold         = 9  UMETA(DisplayName = "Cold Effect"),
	Gotham       = 10 UMETA(DisplayName = "Gotham Effect"),
	Sharpen      = 11 UMETA(DisplayName = "Sharpen Effect"),
	Detail       = 12 UMETA(DisplayName = "Detail Effect"),
	Invert       = 13 UMETA(DisplayName = "Invert Effect"),
	Stylize      = 14 UMETA(DisplayName = "Stylize Effect"),
	Ghost        = 15 UMETA(DisplayName = "Ghost Effect"),
	None         = 16 UMETA(DisplayName = "No Effect")
};

/**
* Hold all effect parameters.
*/
USTRUCT(BlueprintType)
struct FIVR_EffectParameters
{
	GENERATED_BODY()

public:
	//Set the First Treshold value
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraMan|Effects|Parameters")
	float pIVR_Treshold01;
	//Set the Second Treshold value
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraMan|Effects|Parameters")
	float pIVR_Treshold02;
	//Set the Aperture Size(Cartoon)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraMan|Effects|Parameters")
	int   pIVR_ApertureSize;
	//Enable some effects to Use the L2 Gradient Value (Cartoon)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraMan|Effects|Parameters")
	bool  pIVR_UseL2Gradient;
	//Set the kernel size for some effects
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraMan|Effects|Parameters")
	int   pIVR_KernelSize;
	//Set the Sigma S value for some effects.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraMan|Effects|Parameters")
	float pIVR_SigmaS;
	//Set the Sigma R value for some effects.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraMan|Effects|Parameters")
	float pIVR_SigmaR;
	//Set the number of interactions for effects that have repeated executions(Blurr).
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraMan|Effects|Parameters")
	int   pIVR_Iteractions;
	//Set the Light Intensity for some effects(Duo Tone).
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraMan|Effects|Parameters")
	int   pIVR_LightIntensity;
	//Set the Tone 01 (R,G or B) value for some effects(Duo Tone). Range 0 - 2
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraMan|Effects|Parameters")
	int   pIVR_ToneChannel01;
	//Set the Tone 02 (R,G or B) value for some effects(Duo Tone). Range 0 - 2
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraMan|Effects|Parameters")
	int   pIVR_ToneChannel02;
	//Set the expression value used for some effects to change the behaviour of the effect.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraMan|Effects|Parameters")
	float pIVR_ExpValue;
	//Set the Shade Factor applyed in some effects
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraMan|Effects|Parameters")
	float pIVR_ShadeFactor;
	//Some effects can generate Grayscale or Colored effects, use this flag to define it.(Pencil Sketch)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraMan|Effects|Parameters")
	bool  pIVR_ColorOutput;
	
	//Set the Color Tones for Ghost Pixelating. Range 0 - 255
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraMan|Effects|Parameters")
	int   pIVR_ToneR;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraMan|Effects|Parameters")
	int   pIVR_ToneG;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraMan|Effects|Parameters")
	int   pIVR_ToneB;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraMan|Effects|Parameters")
	int   pIVR_Transparency;
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
	* Support Function to Concatenate two Video files in a output video.
	*
	* Example of Use:
	*
	* IVR_CatVideo("VideoInput01.mp4","VideoInput02.mp4","VideoOutput.mp4");
	*/
	UFUNCTION(Category = "CameraMan|Video|Functions", BlueprintCallable)
	static bool IVR_CatVideo(FString pIVR_VideoFPath01, FString pIVR_VideoFPath02, FString pIVR_OutputFullPath)
	{
		//Oops, you dont have a media handler initialized...hummm
		if (!pIVR_MediaHandler)return false;

		QString pVid01Name = QString(TCHAR_TO_UTF8(*pIVR_VideoFPath01));
		QString pVid02Name = QString(TCHAR_TO_UTF8(*pIVR_VideoFPath02));
		QString pOutName   = QString(TCHAR_TO_UTF8(*pIVR_OutputFullPath));

		if (!pIVR_MediaHandler->IVR_CatMedia(pVid01Name, pVid02Name, pOutName))
		{
			//the low level system have problems to Concatenate the files...
			return false;
		}

		//All Good, check it out your file. :)
		return true;
	}

	/**
	* Support Function to ReMux a Video(Good to convert video files in other formats).
	* The list of sucessfull formats used with this function are:
	* .mp4 | .vlc | .mkv | .mov | .mp2 / .mp3 | .wmv | .mpg
	* 
	* Example of Use:
	* 
	* IVR_ReMuxVideo("VideoInput.mp4","VideoOutput.mkv");
	*/
	UFUNCTION(Category = "CameraMan|Video|Functions", BlueprintCallable)
	static bool IVR_ReMuxVideo(FString pIVR_VideoFullPath, FString pIVR_OutputFullPath)
	{
		//Oops, you dont have a media handler initialized...hummm
		if (!pIVR_MediaHandler)return false;

		QString pVidName = QString(TCHAR_TO_UTF8(*pIVR_VideoFullPath));
		QString pOutName = QString(TCHAR_TO_UTF8(*pIVR_OutputFullPath));

		if (!pIVR_MediaHandler->IVR_RemuxMedia(pVidName, pOutName))
		{
			//the low level system have problems to ReMux the file...
			return false;
		}

		//All Good, check it out your file. :)
		return true;
	}

	/**
	* Support Function to Mix a Take(Video without sound) and Insert the sound on it.
	* 
	* Example of use:
	* 
	* IVR_MuxVideo("VideoInput.mp4" ,"AudioInput.mp3","VideoOutput.mp4");
	*/
	UFUNCTION(Category = "CameraMan|Video|Functions", BlueprintCallable)
	static bool IVR_MuxVideo(FString pIVR_VideoFullPath, FString pIVR_AudioFullPath, FString pIVR_OutputFullPath)
	{
		//Oops, you dont have a media handler initialized...hummm
		if (!pIVR_MediaHandler)return false;

		QString pVidName = QString(TCHAR_TO_UTF8(*pIVR_VideoFullPath));
		QString pAudName = QString(TCHAR_TO_UTF8(*pIVR_AudioFullPath));
		QString pOutName = QString(TCHAR_TO_UTF8(*pIVR_OutputFullPath));

		if (!pIVR_MediaHandler->IVR_MuxMedia(pVidName, pAudName, pOutName))
		{
			//the low level system have problems to Mux the file...
			return false;
		}

		//All Good, check it out your file. :)
		return true;
	}

	/**
	* Support Function to Demux a Video(Video with sound) and Record them separetelly.
	*
	* Example of use:
	*
	* IVR_DeMuxVideo("VideoInput.mp4" ,"AudioOutPut.mp3","VideoOutput.mp4");
	*/
	UFUNCTION(Category = "CameraMan|Video|Functions", BlueprintCallable)
	static bool IVR_DemuxVideo(FString pIVR_VideoFullPath, FString pIVR_AudioFullPath, FString pIVR_OutputFullPath)
	{
		//Oops, you dont have a media handler initialized...hummm
		if (!pIVR_MediaHandler)return false;

		QString pVidName = QString(TCHAR_TO_UTF8(*pIVR_VideoFullPath));
		QString pAudName = QString(TCHAR_TO_UTF8(*pIVR_AudioFullPath));
		QString pOutName = QString(TCHAR_TO_UTF8(*pIVR_OutputFullPath));

		if (!pIVR_MediaHandler->IVR_DemuxMedia(pVidName, pAudName, pOutName))
		{
			//the low level system have problems to Mux the file...
			return false;
		}

		//All Good, check it out your file. :)
		return true;
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
	UFUNCTION(Category = "CameraMan|Settings|Functions", BlueprintCallable)
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
		
		if (pIVR_LowLevelActive)
		{
			pIVR_MediaHandler = new CIVRMediaHandler();
		}
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
	static int                    pIVR_GlobalRecording;
	static bool                   pIVR_GlobalReset;
	static bool                   pIVR_DrawDebbug;

	static FCriticalSection       pIVR_CriticalSection;
	static FString                pIVR_FrameFilesPath;

	//Settings Attributes
	static FConfigFile            pIVR_ProjectConfigFile;

	//Low Level SDK Attributes
	static IVRLowLevelSDK*        pIVR_LowLevelInterface;
	static bool                   pIVR_LowLevelActive;

	//Low Level Media Handle
	static CIVRMediaHandler*      pIVR_MediaHandler;

};
