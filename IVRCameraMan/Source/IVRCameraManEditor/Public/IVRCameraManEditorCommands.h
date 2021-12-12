/************************************************************************/
/*Project              :IVR CameraMan - Unreal Plugin                   */
/*Creation Date/Author :William Wolff - 02/18/2021                      */
/*                                                                      */
/*Copyright (c) 2021 William Wolff. All rights reserved                 */
/************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "IVRCameraManEditorStyle.h"

class FIVRCameraManEditorCommands : public TCommands<FIVRCameraManEditorCommands>
{
public:

	FIVRCameraManEditorCommands()
		: TCommands<FIVRCameraManEditorCommands>(TEXT("IVRCameraManEditor"), NSLOCTEXT("Contexts", "IVRCameraManEditor", "IVRCameraManEditor Plugin"), NAME_None, FIVRCameraManEditorStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};