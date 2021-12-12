/************************************************************************/
/*Project              :IVR CameraMan - Unreal Plugin                   */
/*Creation Date/Author :William Wolff - 02/18/2021                      */
/*                                                                      */
/*Copyright (c) 2021 William Wolff. All rights reserved                 */
/************************************************************************/

#include "IVRCameraManEditor.h"
#include "IVRCameraManEditorStyle.h"
#include "IVRCameraManEditorCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"
#include "IVR_WidgetBuilder.h"
#include "IVR_StabVideoWidget.h"
#include "IVR_SuperVideoWidget.h"



static const FName IVRCameraManEditorTabName("IVRCameraManEditor");

#define LOCTEXT_NAMESPACE "FIVRCameraManEditorModule"

void FIVRCameraManEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FIVRCameraManEditorStyle::Initialize();
	FIVRCameraManEditorStyle::ReloadTextures();

	FIVRCameraManEditorCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FIVRCameraManEditorCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FIVRCameraManEditorModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FIVRCameraManEditorModule::RegisterMenus));
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(IVRCameraManEditorTabName, FOnSpawnTab::CreateRaw(this, &FIVRCameraManEditorModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FIVRCameraManEditorTabTitle", "IVRCameraManEditor"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FIVRCameraManEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FIVRCameraManEditorStyle::Shutdown();

	FIVRCameraManEditorCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(IVRCameraManEditorTabName);
}

TSharedRef<SDockTab> FIVRCameraManEditorModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	FText WidgetText = FText::Format(
		LOCTEXT("WindowWidgetText", "Add code to {0} in {1} to override this window's contents"),
		FText::FromString(TEXT("FIVRCameraManEditorModule::OnSpawnPluginTab")),
		FText::FromString(TEXT("IVRCameraManEditor.cpp"))
		);

	UIVR_WidgetBuilder* ScreenBuilder = NewObject<UIVR_WidgetBuilder>(GetTransientPackage(), UIVR_WidgetBuilder::StaticClass());
	FName BrushName;

	BrushName = TEXT("Icon128");

	if (ScreenBuilder->IsValidLowLevel())
	{
		ScreenBuilder->BuildWidget();

		return SNew(SDockTab)
			.TabRole(ETabRole::NomadTab)
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
   			    .HAlign(HAlign_Fill)
			    .VAlign(VAlign_Fill)
			    [
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					[
						SNew(SHorizontalBox) + SHorizontalBox::Slot()
			            [
						   SNew(SVerticalBox) + SVerticalBox::Slot().AutoHeight()
			               [
				              ScreenBuilder->GetRef()
			               ]
						]
		                + SHorizontalBox::Slot()
						[
		                   SNew(SVerticalBox) + SVerticalBox::Slot().AutoHeight()
			               [
				              ScreenBuilder->GetSuperRef()
			               ]
		                   + SVerticalBox::Slot().AutoHeight()
			               [
							   SNew(SHorizontalBox) + SHorizontalBox::Slot()
							   [
				                  SNew(IVR_SuperVideoWidget)
							   ]
						       + SHorizontalBox::Slot()
							   [
								  SNew(IVR_StabVideoWidget)
							   ]
			               ]
						]
					]
			    ]
			];
	}
	
	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			// Put your tab content here!
			SNew(SBox)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(WidgetText)
			]
		];
}

void FIVRCameraManEditorModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(IVRCameraManEditorTabName);
}

void FIVRCameraManEditorModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FIVRCameraManEditorCommands::Get().OpenPluginWindow, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Settings");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FIVRCameraManEditorCommands::Get().OpenPluginWindow));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FIVRCameraManEditorModule, IVRCameraManEditor)