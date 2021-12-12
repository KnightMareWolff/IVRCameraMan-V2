/************************************************************************/
/*Project              :IVR CameraMan - Unreal Plugin                   */
/*Creation Date/Author :William Wolff - 02/18/2021                      */
/*                                                                      */
/*Copyright (c) 2021 William Wolff. All rights reserved                 */
/************************************************************************/
#pragma once

#include "Engine.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "IVR_FunctionLibrary.h" //The Function Library already have the OpenCV Includes

#include "IVRCameraManSystem.generated.h"

/**
 * 
 */
UCLASS()
class IVRCAMERAMAN_API UIVRVideoSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
    
	// Triggered when starting the application
	void Initialize(FSubsystemCollectionBase& Collection) override;
	void Deinitialize() override;

	static bool IsSimulationRunning;	
};
