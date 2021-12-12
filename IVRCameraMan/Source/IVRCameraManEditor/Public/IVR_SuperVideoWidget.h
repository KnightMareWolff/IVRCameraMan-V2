/************************************************************************/
/*Project              :IVR CameraMan - Unreal Plugin                   */
/*Creation Date/Author :William Wolff - 02/18/2021                      */
/*                                                                      */
/*Copyright (c) 2021 William Wolff. All rights reserved                 */
/************************************************************************/
#pragma once

#include "SlateBasics.h"
#include "IVRCameraManEditorStyle.h"
#include "IVRCameraManEditorSubsystem.h"

#define LOCTEXT_NAMESPACE "IVR_SuperVideoWidget"

class IVR_SuperVideoWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(IVR_SuperVideoWidget) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs)
	{
		ChildSlot
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
			    [
					SNew(SButton).OnClicked(this, &IVR_SuperVideoWidget::ButtonPressed)
					.DesiredSizeScale(FVector2D(1.0, 1.0))
			        //.ContentPadding(FMargin(4.0, 2.0))
			        [
				        SNew(SImage).Image(this, &IVR_SuperVideoWidget::GetImageBrush)
			        ]
				 ]
			];
	}

	const FSlateBrush* GetImageBrush() const
	{
		FName BrushName;

		BrushName = TEXT("SuperResolutionBatch");

		return FIVRCameraManEditorStyle::Get().GetBrush(BrushName);
	}

	FReply ButtonPressed()
	{
		UIVRCameraManEditorSubsystem::SuperResolution();
		return FReply::Handled();
	}
};

#undef LOCTEXT_NAMESPACE