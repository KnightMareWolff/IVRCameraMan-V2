// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class IVRCameraMan : ModuleRules
{
	public IVRCameraMan(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				"Runtime/AudioMixer/Public",
				"../Plugins/Runtime/AudioCapture/Source/AudioCapture/Public",
				"../Plugins/Compositing/Composure/Source/Composure/Public",
				"Runtime/Core/Public",
				"Runtime/Projects/Public",
				"Runtime/Engine/Public",
				"Runtime/RHI/Public",
				"Runtime/RenderCore/Public",
				"Runtime/CoreUObject/Public",
				"Runtime/CinematicCamera/Public"
				// ... add public include paths required here ...

			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"IVRCVHelper",
				"IVRLowLevelSDK",
				"Core",
				"Projects",
				"Engine",
				"RHI",
				"RenderCore",
				"CoreUObject",
				"AudioCapture",
				"AudioMixer",
				"CinematicCamera",
				"Slate",
				"SlateCore",
				"UMG",
				"Composure"
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				// ... add private dependencies that you statically link with here ...	
			}
			);


		PublicDefinitions.Add("_CRT_SECURE_NO_WARNINGS");

		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
