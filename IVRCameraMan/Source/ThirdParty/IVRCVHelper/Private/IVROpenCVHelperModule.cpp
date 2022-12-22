/************************************************************************/
/*Project              :IVR CameraMan - Unreal Plugin                   */
/*Creation Date/Author :William Wolff - 02/18/2021                      */
/*                                                                      */
/*Copyright (c) 2021 William Wolff. All rights reserved                 */
/************************************************************************/
#include "IVROpenCVHelperModule.h"
#include "Modules/ModuleManager.h" // for IMPLEMENT_MODULE()
#include "Interfaces/IPluginManager.h"
#include "HAL/PlatformProcess.h"
#include "Core.h"

class FIVRCVHelperModule : public IVRCVHelperModule
{
public:
	FIVRCVHelperModule();

public:
	//~ IModuleInterface interface
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:

	//OpenCV handler
	void* OpenCvDllHandle;

	//IVR Low Level SDK handler
	void* OpenLvDllHandle;

	//Qt handlers
	void* OpenQ1DllHandle;
	void* OpenQ2DllHandle;
	void* OpenQ3DllHandle;
	void* OpenQ4DllHandle;

	//ffmpeg handlers
	void* OpenF1DllHandle;
	void* OpenF2DllHandle;
	void* OpenF3DllHandle;
	void* OpenF4DllHandle;
	void* OpenF5DllHandle;
	void* OpenF6DllHandle;
	void* OpenF7DllHandle;

	//quickLZ handlers
	void* OpenQLDllHandle;
};

FIVRCVHelperModule::FIVRCVHelperModule()
	: OpenCvDllHandle(nullptr)
{}

void FIVRCVHelperModule::StartupModule()
{
	const FString PluginDir = IPluginManager::Get().FindPlugin(TEXT("IVRCameraMan"))->GetBaseDir();

#if WITH_OPENCV
	const FString OpenCvBinPath = PluginDir / TEXT(PREPROCESSOR_TO_STRING(OPENCV_PLATFORM_PATH));
	const FString DLLPath = OpenCvBinPath / TEXT(PREPROCESSOR_TO_STRING(OPENCV_DLL_NAME));
	const FString DLVPath = OpenCvBinPath / TEXT(PREPROCESSOR_TO_STRING(OPENLV_DLL_NAME));
	const FString DQ1Path = OpenCvBinPath / TEXT(PREPROCESSOR_TO_STRING(OPENQ1_DLL_NAME));
	const FString DQ2Path = OpenCvBinPath / TEXT(PREPROCESSOR_TO_STRING(OPENQ2_DLL_NAME));
	const FString DQ3Path = OpenCvBinPath / TEXT(PREPROCESSOR_TO_STRING(OPENQ3_DLL_NAME));
	const FString DQ4Path = OpenCvBinPath / TEXT(PREPROCESSOR_TO_STRING(OPENQ4_DLL_NAME));

	const FString DF1Path = OpenCvBinPath / TEXT(PREPROCESSOR_TO_STRING(FPEGF1_DLL_NAME));
	const FString DF2Path = OpenCvBinPath / TEXT(PREPROCESSOR_TO_STRING(FPEGF2_DLL_NAME));
	const FString DF3Path = OpenCvBinPath / TEXT(PREPROCESSOR_TO_STRING(FPEGF3_DLL_NAME));
	const FString DF4Path = OpenCvBinPath / TEXT(PREPROCESSOR_TO_STRING(FPEGF4_DLL_NAME));
	const FString DF5Path = OpenCvBinPath / TEXT(PREPROCESSOR_TO_STRING(FPEGF5_DLL_NAME));
	const FString DF6Path = OpenCvBinPath / TEXT(PREPROCESSOR_TO_STRING(FPEGF6_DLL_NAME));
	const FString DF7Path = OpenCvBinPath / TEXT(PREPROCESSOR_TO_STRING(FPEGF7_DLL_NAME));

	const FString QL1Path = OpenCvBinPath / TEXT(PREPROCESSOR_TO_STRING(QLIBF1_DLL_NAME));

	FPlatformProcess::PushDllDirectory(*OpenCvBinPath);
	OpenCvDllHandle = FPlatformProcess::GetDllHandle(*DLLPath);

	if (OpenCvDllHandle)
	{
		// Call the test function in the third party library that opens a message box
		//FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("Load OpenCV Properly")));
	}
	else
	{
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("OpenCV Load Error")));
	}

	OpenLvDllHandle = FPlatformProcess::GetDllHandle(*DLVPath);

	if (OpenLvDllHandle)
	{
		// Call the test function in the third party library that opens a message box
		//FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("Load OpenCV Properly")));
	}
	else
	{
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("IVRLowLevelSDK Load Error")));
	}

	OpenQ1DllHandle = FPlatformProcess::GetDllHandle(*DQ1Path);

	if (OpenQ1DllHandle)
	{
		// Call the test function in the third party library that opens a message box
		//FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("Load OpenCV Properly")));
	}
	else
	{
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("Qt Core Load Error")));
	}

	OpenQ2DllHandle = FPlatformProcess::GetDllHandle(*DQ2Path);

	if (OpenQ2DllHandle)
	{
		// Call the test function in the third party library that opens a message box
		//FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("Load OpenCV Properly")));
	}
	else
	{
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("Qt GUI Load Error")));
	}

	OpenQ3DllHandle = FPlatformProcess::GetDllHandle(*DQ3Path);

	if (OpenQ3DllHandle)
	{
		// Call the test function in the third party library that opens a message box
		//FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("Load OpenCV Properly")));
	}
	else
	{
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("Qt Widgets Load Error")));
	}

	OpenQ4DllHandle = FPlatformProcess::GetDllHandle(*DQ4Path);

	if (OpenQ4DllHandle)
	{
		// Call the test function in the third party library that opens a message box
		//FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("Load OpenCV Properly")));
	}
	else
	{
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("Qt Concurrent Load Error")));
	}

	OpenF1DllHandle = FPlatformProcess::GetDllHandle(*DF1Path);

	if (OpenF1DllHandle)
	{
		// Call the test function in the third party library that opens a message box
		//FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("Load OpenCV Properly")));
	}
	else
	{
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("ffmpeg 01 Load Error")));
	}

	OpenF2DllHandle = FPlatformProcess::GetDllHandle(*DF2Path);

	if (OpenF2DllHandle)
	{
		// Call the test function in the third party library that opens a message box
		//FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("Load OpenCV Properly")));
	}
	else
	{
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("ffmpeg 02 Load Error")));
	}

	OpenF3DllHandle = FPlatformProcess::GetDllHandle(*DF3Path);

	if (OpenF3DllHandle)
	{
		// Call the test function in the third party library that opens a message box
		//FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("Load OpenCV Properly")));
	}
	else
	{
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("ffmpeg 03 Load Error")));
	}

	OpenF4DllHandle = FPlatformProcess::GetDllHandle(*DF4Path);

	if (OpenF4DllHandle)
	{
		// Call the test function in the third party library that opens a message box
		//FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("Load OpenCV Properly")));
	}
	else
	{
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("ffmpeg 04 Load Error")));
	}

	OpenF5DllHandle = FPlatformProcess::GetDllHandle(*DF5Path);

	if (OpenF5DllHandle)
	{
		// Call the test function in the third party library that opens a message box
		//FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("Load OpenCV Properly")));
	}
	else
	{
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("ffmpeg 05 Load Error")));
	}

	OpenF6DllHandle = FPlatformProcess::GetDllHandle(*DF6Path);

	if (OpenF6DllHandle)
	{
		// Call the test function in the third party library that opens a message box
		//FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("Load OpenCV Properly")));
	}
	else
	{
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("ffmpeg 06 Load Error")));
	}

	OpenF7DllHandle = FPlatformProcess::GetDllHandle(*DF7Path);

	if (OpenF7DllHandle)
	{
		// Call the test function in the third party library that opens a message box
		//FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("Load OpenCV Properly")));
	}
	else
	{
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("ffmpeg 07 Load Error")));
	}

	OpenQLDllHandle = FPlatformProcess::GetDllHandle(*QL1Path);
	if (OpenQLDllHandle)
	{
		// Call the test function in the third party library that opens a message box
		//FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("Load OpenCV Properly")));
	}
	else
	{
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("quickLZ Load Error")));
	}

	FPlatformProcess::PopDllDirectory(*OpenCvBinPath);
#endif
}

void FIVRCVHelperModule::ShutdownModule()
{
#if WITH_OPENCV
	if (OpenCvDllHandle)
	{
		//FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("OpenCV Was Deinitialized!")));
		FPlatformProcess::FreeDllHandle(OpenCvDllHandle);
		OpenCvDllHandle = nullptr;
	}
#endif
}

IMPLEMENT_MODULE(FIVRCVHelperModule, IVRCVHelper);
