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

/**
 *The Global Config File Handle the initial parameters for the Low Level Cams and system configurations.
 */
FConfigFile     UIVR_FunctionLibrary::pIVR_ProjectConfigFile;

/**
 *The Global Media Handler Make possible we manipulate media (Video Files , audio and Text) using industry codecs/decoders.
 *This attribute are expected to be used in cases we need manipulate media outside of a simulation and rendering loops.
 *If you need make a real-time manipulation during your recording i suggest you wait for some upcomming implementations or
 *put this inside of a AsyncTask to not block your FPS.
 */
CIVRMediaHandler* UIVR_FunctionLibrary::pIVR_MediaHandler = nullptr;

