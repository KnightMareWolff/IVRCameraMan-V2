/************************************************************************/
/*Project              :IVR CameraMan - Unreal Plugin                   */
/*Creation Date/Author :William Wolff - 02/18/2021                      */
/*                                                                      */
/*Copyright (c) 2021 William Wolff. All rights reserved                 */
/************************************************************************/
#include "IVR_FunctionLibrary.h"

//All global objects must be decleared in the CPP file to avoid errors...

/**
 *The Global Recording signal is the main place where the AI Camera Mans will look for to know if they must start to
 * record something or not.
 * WARNING!
 * In Future we can have more signals to individual cameras etc...so dont forget enable them before see the recordings.. ;)
 * 0= StartRecord 1=StopRecord 2=Iddle
 */
int  UIVR_FunctionLibrary::pIVR_GlobalRecording = 2;
bool UIVR_FunctionLibrary::pIVR_GlobalReset     = false;
bool UIVR_FunctionLibrary::pIVR_DrawDebbug      = true;


/**
 *The Global Critical Section makes some critical processes be ThreadSafe during a Background Process.
 * This is Critical for the runtime performance since we will record many video frames at Backgrount.
 * WARNING!
 * In Future its possible we create more critical areas to handle video streamming... ;)
 */
FCriticalSection  UIVR_FunctionLibrary::pIVR_CriticalSection;

/**
 *The Global Frame Files Path will be used to record , read frames and generate all videos.
 * In Future its possible we create a tool to configure this parameters... ;)
 */
FString           UIVR_FunctionLibrary::pIVR_FrameFilesPath = FPaths::ProjectDir() / "Plugins" / "IVRCameraMan" / "Recordings";

/**
 *The Global LowLevel Atributes take handle of the control of Low Level functions like.
 *Record Images , Read Images , Filtering etc... ;)
 */
IVRLowLevelSDK* UIVR_FunctionLibrary::pIVR_LowLevelInterface = nullptr;
bool            UIVR_FunctionLibrary::pIVR_LowLevelActive    = false;


FConfigFile     UIVR_FunctionLibrary::pIVR_ProjectConfigFile;

