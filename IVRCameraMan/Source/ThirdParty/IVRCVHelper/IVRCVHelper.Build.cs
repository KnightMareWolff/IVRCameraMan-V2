// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class IVRCVHelper: ModuleRules
{
	public IVRCVHelper(ReadOnlyTargetRules Target) : base(Target)
	{
		PublicIncludePaths.AddRange(
			new string[] {
			}
		);

		PrivateIncludePaths.AddRange(
			new string[] {
				//"IVRCVHelper/Private"
			}
		);

		PublicDependencyModuleNames.AddRange(
			new string[] {
            }
        );

		PrivateDependencyModuleNames.AddRange(
			new string[] {
				"Core",
				"CoreUObject",
				"Engine",
                "Projects",
                "IVRLowLevelSDK"
            }
        );

		PublicDefinitions.Add("_CRT_SECURE_NO_WARNINGS");
		//PrivatePCHHeaderFile = "Public/IVROpenCVHelperModule.h";
	}
}
