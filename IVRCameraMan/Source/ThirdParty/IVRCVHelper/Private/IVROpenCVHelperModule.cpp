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
	void* OpenCvDllHandle;
	void* OpenLvDllHandle;
	void* OpenQ1DllHandle;
	void* OpenQ2DllHandle;
	void* OpenQ3DllHandle;
	void* OpenQ4DllHandle;
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
