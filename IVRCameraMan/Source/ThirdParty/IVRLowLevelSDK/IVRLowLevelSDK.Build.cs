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
			string FpgPath = Path.Combine(ModuleDirectory, "include/ffmpeg");
			string QlzPath = Path.Combine(ModuleDirectory, "include/quicklz");

			PublicSystemIncludePaths.Add(IncPath);
			PublicSystemIncludePaths.Add(FpgPath);
			PublicSystemIncludePaths.Add(QlzPath);

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

			string F1LibName = "avcodec";
			string F2LibName = "avutil";
			string F3LibName = "avdevice";
			string F4LibName = "avfilter";
			string F5LibName = "avformat";
			string F6LibName = "swresample";
			string F7LibName = "swscale";

			string QLLibName = "quicklz150_64_1";

			PublicAdditionalLibraries.Add(Path.Combine(LibPath, CVLibName + ".lib"));
			PublicAdditionalLibraries.Add(Path.Combine(LibPath, LVLibName + ".lib"));

			PublicAdditionalLibraries.Add(Path.Combine(LibPath, Q1LibName + ".lib"));
			PublicAdditionalLibraries.Add(Path.Combine(LibPath, Q2LibName + ".lib"));
			PublicAdditionalLibraries.Add(Path.Combine(LibPath, Q3LibName + ".lib"));
			PublicAdditionalLibraries.Add(Path.Combine(LibPath, Q4LibName + ".lib"));

			PublicAdditionalLibraries.Add(Path.Combine(LibPath, F1LibName + ".lib"));
			PublicAdditionalLibraries.Add(Path.Combine(LibPath, F2LibName + ".lib"));
			PublicAdditionalLibraries.Add(Path.Combine(LibPath, F3LibName + ".lib"));
			PublicAdditionalLibraries.Add(Path.Combine(LibPath, F4LibName + ".lib"));
			PublicAdditionalLibraries.Add(Path.Combine(LibPath, F5LibName + ".lib"));
			PublicAdditionalLibraries.Add(Path.Combine(LibPath, F6LibName + ".lib"));
			PublicAdditionalLibraries.Add(Path.Combine(LibPath, F7LibName + ".lib"));

			PublicAdditionalLibraries.Add(Path.Combine(LibPath, QLLibName + ".lib"));

			string CVDLLName = CVLibName + ".dll";
			string LVDLLName = LVLibName + ".dll";

			string Q1DLLName = Q1LibName + ".dll";
			string Q2DLLName = Q2LibName + ".dll";
			string Q3DLLName = Q3LibName + ".dll";
			string Q4DLLName = Q4LibName + ".dll";

			//For FFMPEG the tool gives different names for the lib->dll depending on the version...
			//so...becouse we need provide the dll names for component loading we need map this names before release.
			//this not happen with the Low-Level SDK enviroment since Qt Creator map this automatically and
			//the plugin load structure are not used in other types of enviroment.
			//F1LibName = "avcodec-58";
			//F2LibName = "avutil-56";
			//F3LibName = "avdevice-58";
			//F4LibName = "avfilter-7";
			//F5LibName = "avformat-58";
			//F6LibName = "swresample-3";
			//F7LibName = "swscale-5";

			string F1DLLName = F1LibName + "-58.dll";
			string F2DLLName = F2LibName + "-56.dll";
			string F3DLLName = F3LibName + "-58.dll";
			string F4DLLName = F4LibName + "-7.dll";
			string F5DLLName = F5LibName + "-58.dll";
			string F6DLLName = F6LibName + "-3.dll";
			string F7DLLName = F7LibName + "-5.dll";

			string QLDLLName = QLLibName + ".dll";

			PublicDelayLoadDLLs.Add(CVDLLName);
			PublicDelayLoadDLLs.Add(LVDLLName);

			PublicDelayLoadDLLs.Add(Q1DLLName);
			PublicDelayLoadDLLs.Add(Q2DLLName);
			PublicDelayLoadDLLs.Add(Q3DLLName);
			PublicDelayLoadDLLs.Add(Q4DLLName);

			PublicDelayLoadDLLs.Add(F1DLLName);
			PublicDelayLoadDLLs.Add(F2DLLName);
			PublicDelayLoadDLLs.Add(F3DLLName);
			PublicDelayLoadDLLs.Add(F4DLLName);
			PublicDelayLoadDLLs.Add(F5DLLName);
			PublicDelayLoadDLLs.Add(F6DLLName);
			PublicDelayLoadDLLs.Add(F7DLLName);

			PublicDelayLoadDLLs.Add(QLDLLName);

			RuntimeDependencies.Add(Path.Combine(BinaryPath, CVDLLName));
			RuntimeDependencies.Add(Path.Combine(BinaryPath, LVDLLName));
			
			RuntimeDependencies.Add(Path.Combine(BinaryPath, Q1DLLName));
			RuntimeDependencies.Add(Path.Combine(BinaryPath, Q2DLLName));
			RuntimeDependencies.Add(Path.Combine(BinaryPath, Q3DLLName));
			RuntimeDependencies.Add(Path.Combine(BinaryPath, Q4DLLName));

			RuntimeDependencies.Add(Path.Combine(BinaryPath, F1DLLName));
			RuntimeDependencies.Add(Path.Combine(BinaryPath, F2DLLName));
			RuntimeDependencies.Add(Path.Combine(BinaryPath, F3DLLName));
			RuntimeDependencies.Add(Path.Combine(BinaryPath, F4DLLName));
			RuntimeDependencies.Add(Path.Combine(BinaryPath, F5DLLName));
			RuntimeDependencies.Add(Path.Combine(BinaryPath, F6DLLName));
			RuntimeDependencies.Add(Path.Combine(BinaryPath, F7DLLName));

			RuntimeDependencies.Add(Path.Combine(BinaryPath, QLDLLName));

			PublicDefinitions.Add("_CRT_SECURE_NO_WARNINGS");

			PublicDefinitions.Add("WITH_OPENCV=1");
			PublicDefinitions.Add("OPENCV_PLATFORM_PATH=Binaries/ThirdParty/IVRLowLevelSDK/" + PlatformDir);
			PublicDefinitions.Add("OPENCV_DLL_NAME=" + CVDLLName);
			PublicDefinitions.Add("OPENLV_DLL_NAME=" + LVDLLName);
			PublicDefinitions.Add("OPENQ1_DLL_NAME=" + Q1DLLName);
			PublicDefinitions.Add("OPENQ2_DLL_NAME=" + Q2DLLName);
			PublicDefinitions.Add("OPENQ3_DLL_NAME=" + Q3DLLName);
			PublicDefinitions.Add("OPENQ4_DLL_NAME=" + Q4DLLName);

			PublicDefinitions.Add("FPEGF1_DLL_NAME=" + F1DLLName);
			PublicDefinitions.Add("FPEGF2_DLL_NAME=" + F2DLLName);
			PublicDefinitions.Add("FPEGF3_DLL_NAME=" + F3DLLName);
			PublicDefinitions.Add("FPEGF4_DLL_NAME=" + F4DLLName);
			PublicDefinitions.Add("FPEGF5_DLL_NAME=" + F5DLLName);
			PublicDefinitions.Add("FPEGF6_DLL_NAME=" + F6DLLName);
			PublicDefinitions.Add("FPEGF7_DLL_NAME=" + F7DLLName);

			PublicDefinitions.Add("QLIBF1_DLL_NAME=" + QLDLLName);

		}
		else // unsupported platform
		{
            PublicDefinitions.Add("WITH_OPENCV=0");
		}
	}
}
