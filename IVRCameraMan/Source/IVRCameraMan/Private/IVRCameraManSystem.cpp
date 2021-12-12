/************************************************************************/
/*Project              :IVR CameraMan - Unreal Plugin                   */
/*Creation Date/Author :William Wolff - 02/18/2021                      */
/*                                                                      */
/*Copyright (c) 2021 William Wolff. All rights reserved                 */
/************************************************************************/
#include "IVRCameraManSystem.h"
#include "IVR_FunctionLibrary.h" //The Function Library already have the OpenCV Includes


bool UIVRVideoSystem::IsSimulationRunning = false;

void UIVRVideoSystem::Initialize(FSubsystemCollectionBase& Collection)
{
    IsSimulationRunning = true;
    
    UIVR_FunctionLibrary::IVR_InitializeLowLevelSystem();

}


void UIVRVideoSystem::Deinitialize()
{
    IsSimulationRunning = false;  

    //If we finish a simulation we clean up the mess... ;)
    UIVR_FunctionLibrary::IVR_ShutdownLowLevelSystem();
}