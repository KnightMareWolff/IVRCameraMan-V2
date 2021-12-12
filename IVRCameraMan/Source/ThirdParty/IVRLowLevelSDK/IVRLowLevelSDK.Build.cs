// Copyright Epic Games, Inc. All Rights Reserved.
using System;
using System.IO;
using UnrealBuildTool;

public class IVRLowLevelSDK : ModuleRules
{
	public IVRLowLevelSDK(ReadOnlyTargetRules Target) : base(Target)
	{
		Type = ModuleType.External;

		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			string PlatformDir = Target.Platform.ToString();
			string IncPath = Path.Combine(ModuleDirectory, "include");

			PublicSystemIncludePaths.Add(IncPath);

			string LibPath = Path.Combine(ModuleDirectory, "lib", PlatformDir);
			string BinaryPath = Path.GetFullPath(Path.Combine(ModuleDirectory, "../../../Binaries/ThirdParty/IVRLowLevelSDK", PlatformDir));

			//For Debug
			//string CVLibName = "opencv_world453d";
			//string LVLibName = "IVRLowLevelSDK";
			//string Q1LibName = "Qt5Cored";
			//string Q2LibName = "Qt5Guid";
			//string Q3LibName = "Qt5Widgetsd";
			//string Q4LibName = "Qt5Concurrentd";

			//For Release
			string CVLibName = "opencv_world454";
			string LVLibName = "IVRLowLevelSDK";
			string Q1LibName = "Qt5Core";
			string Q2LibName = "Qt5Gui";
			string Q3LibName = "Qt5Widgets";
			string Q4LibName = "Qt5Concurrent";

			PublicAdditionalLibraries.Add(Path.Combine(LibPath, CVLibName + ".lib"));
			PublicAdditionalLibraries.Add(Path.Combine(LibPath, LVLibName + ".lib"));
			PublicAdditionalLibraries.Add(Path.Combine(LibPath, Q1LibName + ".lib"));
			PublicAdditionalLibraries.Add(Path.Combine(LibPath, Q2LibName + ".lib"));
			PublicAdditionalLibraries.Add(Path.Combine(LibPath, Q3LibName + ".lib"));
			PublicAdditionalLibraries.Add(Path.Combine(LibPath, Q4LibName + ".lib"));

			string CVDLLName = CVLibName + ".dll";
			string LVDLLName = LVLibName + ".dll";
			string Q1DLLName = Q1LibName + ".dll";
			string Q2DLLName = Q2LibName + ".dll";
			string Q3DLLName = Q3LibName + ".dll";
			string Q4DLLName = Q4LibName + ".dll";

			PublicDelayLoadDLLs.Add(CVDLLName);
			PublicDelayLoadDLLs.Add(LVDLLName);
			PublicDelayLoadDLLs.Add(Q1DLLName);
			PublicDelayLoadDLLs.Add(Q2DLLName);
			PublicDelayLoadDLLs.Add(Q3DLLName);
			PublicDelayLoadDLLs.Add(Q4DLLName);

			RuntimeDependencies.Add(Path.Combine(BinaryPath, CVDLLName));
			RuntimeDependencies.Add(Path.Combine(BinaryPath, LVDLLName));
			RuntimeDependencies.Add(Path.Combine(BinaryPath, Q1DLLName));
			RuntimeDependencies.Add(Path.Combine(BinaryPath, Q2DLLName));
			RuntimeDependencies.Add(Path.Combine(BinaryPath, Q3DLLName));
			RuntimeDependencies.Add(Path.Combine(BinaryPath, Q4DLLName));

			PublicDefinitions.Add("WITH_OPENCV=1");
			PublicDefinitions.Add("OPENCV_PLATFORM_PATH=Binaries/ThirdParty/IVRLowLevelSDK/" + PlatformDir);
			PublicDefinitions.Add("OPENCV_DLL_NAME=" + CVDLLName);
			PublicDefinitions.Add("OPENLV_DLL_NAME=" + LVDLLName);
			PublicDefinitions.Add("OPENQ1_DLL_NAME=" + Q1DLLName);
			PublicDefinitions.Add("OPENQ2_DLL_NAME=" + Q2DLLName);
			PublicDefinitions.Add("OPENQ3_DLL_NAME=" + Q3DLLName);
			PublicDefinitions.Add("OPENQ4_DLL_NAME=" + Q4DLLName);

		}
		else // unsupported platform
		{
            PublicDefinitions.Add("WITH_OPENCV=0");
		}
	}
}
