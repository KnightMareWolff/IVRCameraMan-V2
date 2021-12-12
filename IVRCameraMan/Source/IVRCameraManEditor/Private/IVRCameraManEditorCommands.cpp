/************************************************************************/
/*Project              :IVR CameraMan - Unreal Plugin                   */
/*Creation Date/Author :William Wolff - 02/18/2021                      */
/*                                                                      */
/*Copyright (c) 2021 William Wolff. All rights reserved                 */
/************************************************************************/

#include "IVRCameraManEditorCommands.h"

#define LOCTEXT_NAMESPACE "FIVRCameraManEditorModule"

void FIVRCameraManEditorCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "IVRCameraManEditor", "Bring up IVRCameraManEditor window", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
