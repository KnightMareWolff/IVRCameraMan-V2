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
#define LOCTEXT_NAMESPACE "IVR_StabVideoWidget"



class IVR_StabVideoWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(IVR_StabVideoWidget) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs)
	{
		ChildSlot
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
			    [
					SNew(SButton).OnClicked(this, &IVR_StabVideoWidget::ButtonPressed)
					.DesiredSizeScale(FVector2D(1.0, 1.0))
			        //.ContentPadding(FMargin(4.0, 2.0))
			        [
				        SNew(SImage).Image(this, &IVR_StabVideoWidget::GetImageBrush)
			        ]
				 ]
			];
	}

	const FSlateBrush* GetImageBrush() const
	{
		FName BrushName;

		BrushName = TEXT("StabilizationBatch");

		return FIVRCameraManEditorStyle::Get().GetBrush(BrushName);
	}

	FReply ButtonPressed()
	{
		//FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("Starting Stabilize the Videos...This will take a lot of Time! \n (All the videos will have Stab at the end of the Name)")));
		UIVRCameraManEditorSubsystem::StabilizeVideos();
		//FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("Great!...All Video Files was Stabilized!")));
		return FReply::Handled();
	}

};

#undef LOCTEXT_NAMESPACE