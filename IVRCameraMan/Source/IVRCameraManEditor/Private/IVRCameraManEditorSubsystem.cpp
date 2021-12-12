/************************************************************************/
/*Project              :IVR CameraMan - Unreal Plugin                   */
/*Creation Date/Author :William Wolff - 02/18/2021                      */
/*                                                                      */
/*Copyright (c) 2021 William Wolff. All rights reserved                 */
/************************************************************************/


#include "IVRCameraManEditorSubsystem.h"

using namespace std;
using namespace cv;
using namespace cv::superres;

bool UIVRCameraManEditorSubsystem::IsEditorRunning = false;

FString          UIVRCameraManEditorSubsystem::IVR_RecordingPath       ;
FString          UIVRCameraManEditorSubsystem::IVR_VideoResolution     ;
int32            UIVRCameraManEditorSubsystem::IVR_Width               ;
int32            UIVRCameraManEditorSubsystem::IVR_Height              ;
FLinearColor     UIVRCameraManEditorSubsystem::IVR_ClearColor          ;
float            UIVRCameraManEditorSubsystem::IVR_Gamma               ;
float            UIVRCameraManEditorSubsystem::IVR_StabilizationArm    ;
float            UIVRCameraManEditorSubsystem::IVR_LagDistance         ;
FString          UIVRCameraManEditorSubsystem::IVR_CameraManName       ;
float            UIVRCameraManEditorSubsystem::IVR_CameraVelocity      ;
float            UIVRCameraManEditorSubsystem::IVR_BaseTurnRate        ;
float            UIVRCameraManEditorSubsystem::IVR_BaseLookUpRate      ;
int32            UIVRCameraManEditorSubsystem::IVR_SuperScale          ;
int32            UIVRCameraManEditorSubsystem::IVR_SuperIterations     ;
int32            UIVRCameraManEditorSubsystem::IVR_SuperTemporalRadius ;
FString          UIVRCameraManEditorSubsystem::IVR_SuperOpticalFlow    ;
bool             UIVRCameraManEditorSubsystem::IVR_DebugRendering      ;
int              UIVRCameraManEditorSubsystem::IVR_MSecToWait          ;

// Define variable for storing frames
cv::Mat* UIVRCameraManEditorSubsystem::StabCurrFrame;
cv::Mat* UIVRCameraManEditorSubsystem::StabCurrFrameGray;
cv::Mat* UIVRCameraManEditorSubsystem::StabPrevFrame;
cv::Mat* UIVRCameraManEditorSubsystem::StabPrevFrameGray;
cv::Mat* UIVRCameraManEditorSubsystem::StabCurrTransformation;
cv::Mat* UIVRCameraManEditorSubsystem::StabPrevTransformation;
cv::Mat* UIVRCameraManEditorSubsystem::StabilizedFrame;
cv::Mat* UIVRCameraManEditorSubsystem::FixRotationMatrix;
cv::Mat* UIVRCameraManEditorSubsystem::FixFrameStabilized;

// Vector from previous and current feature points
std::vector<cv::Point2f> UIVRCameraManEditorSubsystem::StabPrevFeaturePoints;
std::vector<cv::Point2f> UIVRCameraManEditorSubsystem::StabCurrFeaturePoints;

// Optical Flow results
std::vector <uchar> UIVRCameraManEditorSubsystem::OptFlowStatus;
std::vector <float> UIVRCameraManEditorSubsystem::OptFlowErrors;

// Pre-define transformation-store array
std::vector<TransformParam> UIVRCameraManEditorSubsystem::StabTransformations;
std::vector<TransformParam> UIVRCameraManEditorSubsystem::SmoothedTranformations;

// Transformation Trajectory
std::vector <Trajectory> UIVRCameraManEditorSubsystem::StabTransfTrajectory;
std::vector <Trajectory> UIVRCameraManEditorSubsystem::StabSmoothTrajectory;

static Ptr<DenseOpticalFlowExt> createOptFlow(FString pIVR_SuperOpticalFlow);

static Ptr<DenseOpticalFlowExt> createOptFlow(FString pIVR_SuperOpticalFlow)
{
	if (pIVR_SuperOpticalFlow  == "Farneback")return createOptFlow_Farneback();
	else if (pIVR_SuperOpticalFlow  == "TVL1")return createOptFlow_DualTVL1();
	else if (pIVR_SuperOpticalFlow  == "Brox")return createOptFlow_Brox_CUDA();
	else if (pIVR_SuperOpticalFlow == "PYRLK")return createOptFlow_PyrLK_CUDA();
	return Ptr<DenseOpticalFlowExt>();
}

void UIVRCameraManEditorSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	IsEditorRunning = true;

    //Just a checking message box...
    //FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("Simulation Start")));

}


void UIVRCameraManEditorSubsystem::Deinitialize()
{
	IsEditorRunning = false;

   
}

void UIVRCameraManEditorSubsystem::ReadCameraManDefaultValues(FString& pIVR_RecordingPath,
	FString& pIVR_VideoResolution,
	FLinearColor& pIVR_ClearColor,
	float& pIVR_Gamma,
	float& pIVR_StabilizationArm,
	float& pIVR_LagDistance,
	FString& pIVR_CameraManName,
	float& pIVR_CameraVelocity,
	float& pIVR_BaseTurnRate,
	float& pIVR_BaseLookUpRate,
	bool& pIVR_DebugRendering,
	int& pIVR_MSecToWait)
{
	const FString SessionConfigFilePath = FPaths::ConvertRelativePathToFull(FPaths::ProjectDir() / "Config" / "IVRCameraMan" / "DefaultCameraMan.ini");
	FString tStr;

	FConfigFile IVR_ProjectConfigFile;

	//If the Configuration File Not Exists we create a new One! (In this way the User dont need worry about it!) ;)
	if (FPlatformFileManager::Get().GetPlatformFile().FileExists(*SessionConfigFilePath))
	{
		IVR_ProjectConfigFile.Read(SessionConfigFilePath);

		const auto SectionName = TEXT("DefaultCameraParameters");

		if (IVR_ProjectConfigFile.GetString(SectionName, TEXT("RecordingPath"), pIVR_RecordingPath) == false)
		{
			UE_LOG(LogTemp, Error, TEXT("Could not load [%s] RecordingPath"), SectionName);
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("%s"), *(pIVR_RecordingPath));
		}

		if (IVR_ProjectConfigFile.GetString(SectionName, TEXT("VideoResolution"), tStr) == false)
		{
			UE_LOG(LogTemp, Error, TEXT("Could not load [%s] RecordingPath"), SectionName);
		}
		else
		{
			pIVR_VideoResolution = tStr;
			UE_LOG(LogTemp, Log, TEXT("%s"), *(tStr));
		}

		if (IVR_ProjectConfigFile.GetString(SectionName, TEXT("Gamma"), tStr) == false)
		{
			UE_LOG(LogTemp, Error, TEXT("Could not load [%s] Gamma"), SectionName);
		}
		else
		{
			pIVR_Gamma = atof(TCHAR_TO_UTF8(*tStr));
			UE_LOG(LogTemp, Log, TEXT("%s"), *(tStr));
		}

		if (IVR_ProjectConfigFile.GetString(SectionName, TEXT("StabilizationArm"), tStr) == false)
		{
			UE_LOG(LogTemp, Error, TEXT("Could not load [%s] StabilizationArm"), SectionName);
		}
		else
		{
			pIVR_StabilizationArm = atof(TCHAR_TO_UTF8(*tStr));
			UE_LOG(LogTemp, Log, TEXT("%s"), *(tStr));
		}

		if (IVR_ProjectConfigFile.GetString(SectionName, TEXT("LagDistance"), tStr) == false)
		{
			UE_LOG(LogTemp, Error, TEXT("Could not load [%s] LagDistance"), SectionName);
		}
		else
		{
			pIVR_LagDistance = atof(TCHAR_TO_UTF8(*tStr));
			UE_LOG(LogTemp, Log, TEXT("%s"), *(tStr));
		}

		if (IVR_ProjectConfigFile.GetString(SectionName, TEXT("CameraManName"), pIVR_CameraManName) == false)
		{
			UE_LOG(LogTemp, Error, TEXT("Could not load [%s] CameraManName"), SectionName);
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("%s"), *(pIVR_CameraManName));
		}

		if (IVR_ProjectConfigFile.GetString(SectionName, TEXT("CameraVelocity"), tStr) == false)
		{
			UE_LOG(LogTemp, Error, TEXT("Could not load [%s] CameraVelocity"), SectionName);
		}
		else
		{
			pIVR_CameraVelocity = atof(TCHAR_TO_UTF8(*tStr));
			UE_LOG(LogTemp, Log, TEXT("%s"), *(tStr));
		}

		if (IVR_ProjectConfigFile.GetString(SectionName, TEXT("BaseTurnRate"), tStr) == false)
		{
			UE_LOG(LogTemp, Error, TEXT("Could not load [%s] BaseTurnRate"), SectionName);
		}
		else
		{
			pIVR_BaseTurnRate = atof(TCHAR_TO_UTF8(*tStr));
			UE_LOG(LogTemp, Log, TEXT("%s"), *(tStr));
		}

		if (IVR_ProjectConfigFile.GetString(SectionName, TEXT("BaseLookUpRate"), tStr) == false)
		{
			UE_LOG(LogTemp, Error, TEXT("Could not load [%s] BaseLookUpRate"), SectionName);
		}
		else
		{
			pIVR_BaseLookUpRate = atof(TCHAR_TO_UTF8(*tStr));
			UE_LOG(LogTemp, Log, TEXT("%s"), *(tStr));
		}

		if (IVR_ProjectConfigFile.GetString(SectionName, TEXT("DebugRendering"), tStr) == false)
		{
			UE_LOG(LogTemp, Error, TEXT("Could not load [%s] Flag"), SectionName);
		}
		else
		{
			pIVR_DebugRendering = tStr.ToBool();
			UE_LOG(LogTemp, Log, TEXT("%s"), *(tStr));
		}

		if (IVR_ProjectConfigFile.GetString(SectionName, TEXT("MSecToWait"), tStr) == false)
		{
			UE_LOG(LogTemp, Error, TEXT("Could not load [%s] MSecToWait"), SectionName);
		}
		else
		{
			pIVR_MSecToWait = atoi(TCHAR_TO_UTF8(*tStr));
			UE_LOG(LogTemp, Log, TEXT("%s"), *(tStr));
		}

		//Update The global System Parameters
		IVR_RecordingPath = pIVR_RecordingPath;

		if (pIVR_VideoResolution == "SD (Standard Definition)"  )IVR_Width = 640 ; IVR_Height = 480;
		if (pIVR_VideoResolution == "HD (High Definition)"      )IVR_Width = 1280; IVR_Height = 720;
		if (pIVR_VideoResolution == "Full HD (FHD)"             )IVR_Width = 1920; IVR_Height = 1080;
		if (pIVR_VideoResolution == "QHD (Quad HD)"             )IVR_Width = 2560; IVR_Height = 1440;
		if (pIVR_VideoResolution == "2K video"                  )IVR_Width = 2048; IVR_Height = 1080;
		if (pIVR_VideoResolution == "4K video or Ultra HD (UHD)")IVR_Width = 3840; IVR_Height = 2160;
		if (pIVR_VideoResolution == "8K video or Full Ultra HD" )IVR_Width = 7680; IVR_Height = 4320;
		
		IVR_ClearColor = pIVR_ClearColor;
		IVR_Gamma = pIVR_Gamma;
		IVR_StabilizationArm = pIVR_StabilizationArm;
		IVR_LagDistance = pIVR_LagDistance;
		IVR_CameraManName = pIVR_CameraManName;
		IVR_CameraVelocity = pIVR_CameraVelocity;
		IVR_BaseTurnRate = pIVR_BaseTurnRate;
		IVR_BaseLookUpRate = pIVR_BaseLookUpRate;
		IVR_DebugRendering = pIVR_DebugRendering;
		IVR_MSecToWait = pIVR_MSecToWait;
	}
	else
	{
		pIVR_RecordingPath = FPaths::ProjectDir() / "Plugins" / "IVRCameraMan" / "Recordings";
		pIVR_VideoResolution = "HD (High Definition)";
		IVR_Width = 1280;
		IVR_Height = 720;
		pIVR_ClearColor = FLinearColor::Black;
		pIVR_Gamma = 1.0f;
		pIVR_StabilizationArm = 0.0f;
		pIVR_LagDistance = 3.0f;
		pIVR_CameraManName = "DefaultCameraMan";
		pIVR_CameraVelocity = 350.0f;
		pIVR_BaseTurnRate = 30.0f;
		pIVR_BaseLookUpRate = 30.0f;
		pIVR_DebugRendering = true;
		pIVR_MSecToWait = 600;

		WriteCameraManDefaultValues(pIVR_RecordingPath,
			pIVR_VideoResolution,
			pIVR_ClearColor,
			pIVR_Gamma,
			pIVR_StabilizationArm,
			pIVR_LagDistance,
			pIVR_CameraManName,
			pIVR_CameraVelocity,
			pIVR_BaseTurnRate,
			pIVR_BaseLookUpRate,
			pIVR_DebugRendering,
			pIVR_MSecToWait);
	}

}

void UIVRCameraManEditorSubsystem::WriteCameraManDefaultValues(FString   pIVR_RecordingPath,
	FString                   pIVR_VideoResolution,
	FLinearColor              pIVR_ClearColor,
	float                     pIVR_Gamma,
	float                     pIVR_StabilizationArm,
	float                     pIVR_LagDistance,
	FString                   pIVR_CameraManName,
	float                     pIVR_CameraVelocity,
	float                     pIVR_BaseTurnRate,
	float                     pIVR_BaseLookUpRate,
	bool                      pIVR_DebugRendering,
	int                       pIVR_MSecToWait)
{
	const FString SessionConfigFilePath = FPaths::ConvertRelativePathToFull(FPaths::ProjectDir() / "Config" / "IVRCameraMan");
	const FString SessionConfigFileName = FPaths::ConvertRelativePathToFull(FPaths::ProjectDir() / "Config" / "IVRCameraMan" / "DefaultCameraMan.ini");
	FString tStr;

	// Holds specific machine configuration file
	FConfigFile SConfigFile;

	IPlatformFile& PF = FPlatformFileManager::Get().GetPlatformFile();

	if (PF.CreateDirectoryTree(*SessionConfigFilePath) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("Could not create/ensure plugin config folder creation!"));
		return;
	}

	//Update The global System Parameters
	IVR_RecordingPath = pIVR_RecordingPath;
	IVR_VideoResolution = pIVR_VideoResolution;

	if (pIVR_VideoResolution == "SD (Standard Definition)") { IVR_Width = 640; IVR_Height = 480; }
	if (pIVR_VideoResolution == "HD (High Definition)") { IVR_Width = 1280; IVR_Height = 720; }
	if (pIVR_VideoResolution == "Full HD (FHD)") { IVR_Width = 1920; IVR_Height = 1080; }
	if (pIVR_VideoResolution == "QHD (Quad HD)") { IVR_Width = 2560; IVR_Height = 1440; }
	if (pIVR_VideoResolution == "2K video") { IVR_Width = 2048; IVR_Height = 1080; }
	if (pIVR_VideoResolution == "4K video or Ultra HD (UHD)") { IVR_Width = 3840; IVR_Height = 2160; }
	if (pIVR_VideoResolution == "8K video or Full Ultra HD") { IVR_Width = 7680; IVR_Height = 4320; }

	IVR_ClearColor = pIVR_ClearColor;
	IVR_Gamma = pIVR_Gamma;
	IVR_StabilizationArm = pIVR_StabilizationArm;
	IVR_LagDistance = pIVR_LagDistance;
	IVR_CameraManName = pIVR_CameraManName;
	IVR_CameraVelocity = pIVR_CameraVelocity;
	IVR_BaseTurnRate = pIVR_BaseTurnRate;
	IVR_BaseLookUpRate = pIVR_BaseLookUpRate;
	IVR_DebugRendering = pIVR_DebugRendering;
	IVR_MSecToWait = pIVR_MSecToWait;

	const auto SectionName = TEXT("DefaultCameraParameters");

	SConfigFile.SetString(SectionName, TEXT("RecordingPath"), *pIVR_RecordingPath);
	SConfigFile.SetString(SectionName, TEXT("VideoResolution"), *pIVR_VideoResolution);
	SConfigFile.SetString(SectionName, TEXT("Gamma"), *FString::SanitizeFloat(pIVR_Gamma));
	SConfigFile.SetString(SectionName, TEXT("StabilizationArm"), *FString::SanitizeFloat(pIVR_StabilizationArm));
	SConfigFile.SetString(SectionName, TEXT("LagDistance"), *FString::SanitizeFloat(pIVR_LagDistance));
	SConfigFile.SetString(SectionName, TEXT("CameraManName"), *pIVR_CameraManName);
	SConfigFile.SetString(SectionName, TEXT("CameraVelocity"), *FString::SanitizeFloat(pIVR_CameraVelocity));
	SConfigFile.SetString(SectionName, TEXT("BaseTurnRate"), *FString::SanitizeFloat(pIVR_BaseTurnRate));
	SConfigFile.SetString(SectionName, TEXT("BaseLookUpRate"), *FString::SanitizeFloat(pIVR_BaseLookUpRate));
	SConfigFile.SetString(SectionName, TEXT("DebugRendering"), pIVR_DebugRendering ? TEXT("true") : TEXT("false"));
	SConfigFile.SetString(SectionName, TEXT("MSecToWait"), *FString::FromInt(pIVR_MSecToWait));

	SConfigFile.Write(SessionConfigFileName);
}

void UIVRCameraManEditorSubsystem::ReadExperimentalParametersValues(int32& pIVR_SuperScale,
	int32& pIVR_SuperIterations,
	int32& pIVR_SuperTemporalRadius,
	FString& pIVR_SuperOpticalFlow)
{
	const FString SessionConfigFilePath = FPaths::ConvertRelativePathToFull(FPaths::ProjectDir() / "Config" / "IVRCameraMan" / "ExperimentalCameraMan.ini");
	FString tStr;

	FConfigFile IVR_ProjectConfigFile;

	//If the Configuration File Not Exists we create a new One! (In this way the User dont need worry about it!) ;)
	if (FPlatformFileManager::Get().GetPlatformFile().FileExists(*SessionConfigFilePath))
	{
		IVR_ProjectConfigFile.Read(SessionConfigFilePath);

		const auto SectionName = TEXT("ExperimentalRecordingParameters");

		if (IVR_ProjectConfigFile.GetString(SectionName, TEXT("Scale"), tStr) == false)
		{
			UE_LOG(LogTemp, Error, TEXT("Could not load [%s] Value"), SectionName);
		}
		else
		{
			pIVR_SuperScale = atoi(TCHAR_TO_UTF8(*tStr));
			UE_LOG(LogTemp, Log, TEXT("%s"), *(tStr));
		}

		if (IVR_ProjectConfigFile.GetString(SectionName, TEXT("Iterations"), tStr) == false)
		{
			UE_LOG(LogTemp, Error, TEXT("Could not load [%s] Value"), SectionName);
		}
		else
		{
			pIVR_SuperIterations = atoi(TCHAR_TO_UTF8(*tStr));
			UE_LOG(LogTemp, Log, TEXT("%s"), *(tStr));
		}

		if (IVR_ProjectConfigFile.GetString(SectionName, TEXT("Radius"), tStr) == false)
		{
			UE_LOG(LogTemp, Error, TEXT("Could not load [%s] Value"), SectionName);
		}
		else
		{
			pIVR_SuperTemporalRadius = atoi(TCHAR_TO_UTF8(*tStr));
			UE_LOG(LogTemp, Log, TEXT("%s"), *(tStr));
		}

		if (IVR_ProjectConfigFile.GetString(SectionName, TEXT("OpticalFlow"), tStr) == false)
		{
			UE_LOG(LogTemp, Error, TEXT("Could not load [%s] Type"), SectionName);
		}
		else
		{
			// Defines what type of camera it should be at start up
			pIVR_SuperOpticalFlow = tStr;
		}

		//Update Subsystem Parameters
		IVR_SuperScale          = pIVR_SuperScale;
		IVR_SuperIterations     = pIVR_SuperIterations;
		IVR_SuperTemporalRadius = pIVR_SuperTemporalRadius;
		IVR_SuperOpticalFlow    = pIVR_SuperOpticalFlow;
		
	}
	else
	{
		pIVR_SuperScale = 2;
		pIVR_SuperIterations = 5;
		pIVR_SuperTemporalRadius = 4;
		pIVR_SuperOpticalFlow = "Farneback";

		WriteExperimentalParametersValues(pIVR_SuperScale,
		pIVR_SuperIterations,
		pIVR_SuperTemporalRadius,
		pIVR_SuperOpticalFlow);
	}

}

void UIVRCameraManEditorSubsystem::WriteExperimentalParametersValues(int32 pIVR_SuperScale,
	int32   pIVR_SuperIterations,
	int32   pIVR_SuperTemporalRadius,
	FString pIVR_SuperOpticalFlow)
{
	const FString SessionConfigFilePath = FPaths::ConvertRelativePathToFull(FPaths::ProjectDir() / "Config" / "IVRCameraMan");
	const FString SessionConfigFileName = FPaths::ConvertRelativePathToFull(FPaths::ProjectDir() / "Config" / "IVRCameraMan" / "ExperimentalCameraMan.ini");
	FString tStr;

	// Holds specific machine configuration file
	FConfigFile SConfigFile;

	IPlatformFile& PF = FPlatformFileManager::Get().GetPlatformFile();

	if (PF.CreateDirectoryTree(*SessionConfigFilePath) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("Could not create/ensure plugin config folder creation!"));
		return;
	}

	//Update Subsystem Parameters
	IVR_SuperScale = pIVR_SuperScale;
	IVR_SuperIterations = pIVR_SuperIterations;
	IVR_SuperTemporalRadius = pIVR_SuperTemporalRadius;
	IVR_SuperOpticalFlow = pIVR_SuperOpticalFlow;

	const auto SectionName = TEXT("ExperimentalRecordingParameters");

	SConfigFile.SetString(SectionName, TEXT("Scale")      , *FString::FromInt(pIVR_SuperScale));
	SConfigFile.SetString(SectionName, TEXT("Iterations") , *FString::FromInt(pIVR_SuperIterations));
	SConfigFile.SetString(SectionName, TEXT("Radius")     , *FString::FromInt(pIVR_SuperTemporalRadius));
	SConfigFile.SetString(SectionName, TEXT("OpticalFlow"), *pIVR_SuperOpticalFlow);
	

	SConfigFile.Write(SessionConfigFileName);
}

void UIVRCameraManEditorSubsystem::cumsum(std::vector<TransformParam>& transforms)
{
	// Accumulated frame to frame transform
	double a = 0;
	double x = 0;
	double y = 0;

	StabTransfTrajectory.clear();

	for (size_t i = 0; i < transforms.size(); i++)
	{
		x += transforms[i].dx;
		y += transforms[i].dy;
		a += transforms[i].da;

		StabTransfTrajectory.push_back(Trajectory(x, y, a));
	}
}

void UIVRCameraManEditorSubsystem::smooth(std::vector<Trajectory>& trajectory, int radius)
{
	StabSmoothTrajectory.clear();

	for (size_t i = 0; i < trajectory.size(); i++) {
		double sum_x = 0;
		double sum_y = 0;
		double sum_a = 0;
		int count = 0;

		for (int j = -radius; j <= radius; j++) {
			if (i + j >= 0 && i + j < trajectory.size()) {
				sum_x += trajectory[i + j].x;
				sum_y += trajectory[i + j].y;
				sum_a += trajectory[i + j].a;

				count++;
			}
		}

		double avg_a = sum_a / count;
		double avg_x = sum_x / count;
		double avg_y = sum_y / count;

		StabSmoothTrajectory.push_back(Trajectory(avg_x, avg_y, avg_a));
	}
}

bool UIVRCameraManEditorSubsystem::ApplyMoveAverageFilter()
{
	// Compute trajectory using cumulative sum of transformations
	cumsum(StabTransformations);
	// Smooth trajectory using moving average filter create the parameter SMOOTHIN RADIUS for the tool
	smooth(StabTransfTrajectory, 15);
	// Finally we calculate each distance to find out every transf to do
	for (size_t i = 0; i < StabTransformations.size(); i++)
	{
		// Calculate difference in smoothed_trajectory and trajectory
		double diff_x = StabSmoothTrajectory[i].x - StabTransfTrajectory[i].x;
		double diff_y = StabSmoothTrajectory[i].y - StabTransfTrajectory[i].y;
		double diff_a = StabSmoothTrajectory[i].a - StabTransfTrajectory[i].a;

		// Calculate newer transformation array
		double pdx = StabTransformations[i].dx + diff_x;
		double pdy = StabTransformations[i].dy + diff_y;
		double pda = StabTransformations[i].da + diff_a;

		SmoothedTranformations.push_back(TransformParam(pdx, pdy, pda));
	}

	//The last transformation is the transf we are interested to get
	SmoothedTranformations[SmoothedTranformations.size() - 1].getTransform(*StabCurrTransformation);

	return true;
}

void UIVRCameraManEditorSubsystem::StabilizeVideos()
{

	FString FramesFilePath = FPaths::ConvertRelativePathToFull(IVR_RecordingPath);
	bool    isSuccess = false;

	TArray<FString> Files;
	const TCHAR* extension = _T("*.mp4");

	IFileManager::Get().FindFilesRecursive(Files, *FramesFilePath, extension, true, false, false);

	StabCurrFrame = new cv::Mat();
	StabPrevFrame = new cv::Mat();
	StabCurrFrameGray = new cv::Mat();
	StabPrevFrameGray = new cv::Mat();
	StabCurrTransformation = new cv::Mat();
	StabPrevTransformation = new cv::Mat();
	StabilizedFrame = new cv::Mat();
	FixRotationMatrix = new cv::Mat();
	FixFrameStabilized = new cv::Mat();

	for (int32 f = 0; f < Files.Num(); f++)
	{
		//Cria Video de entrada com os frames gravados.
		cv::VideoCapture cap(TCHAR_TO_UTF8(*Files[f]));

		// if not success, exit program
		if (cap.isOpened() == false)
		{
			FString Message = "There is no recording for the file = " + Files[f] + "!";

			FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(Message));
			continue;
		}

		int frame_width = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_WIDTH)); //get the width of frames of the video
		int frame_height = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_HEIGHT)); //get the height of frames of the video
		int frames_per_second = static_cast<int>(cap.get(cv::CAP_PROP_FPS));

		cv::Size frame_size(frame_width, frame_height);

		FString VideoPath = FramesFilePath + "/" + "Stabilized" + FString::FromInt(f) + "Output.mp4";

		//Create and initialize the VideoWriter object
		cv::VideoWriter oVideoWriter(TCHAR_TO_UTF8(*VideoPath),
			cv::VideoWriter::fourcc('3', 'I', 'V', 'D'),
			frames_per_second,
			frame_size,
			true);

		//If the VideoWriter object is not initialized successfully, exit the program
		if (oVideoWriter.isOpened() == false)
		{
			FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("Video Capture was not deinitialized properly.")));
			return;
		}

		uint FrameNumber = 0;

		isSuccess = cap.read(*StabPrevFrame); // read a new frame from the video camera
		//Reading the first frame and consider it as a previous frame
		if (isSuccess == false)
		{
			oVideoWriter.release();
			cap.release();
			break;
		}

		while (true)
		{

			//now when we read the frame again we have the current and previous Frame
			isSuccess = cap.read(*StabCurrFrame);
			if (isSuccess == false)
			{
				oVideoWriter.release();
				cap.release();
				break;
			}

			/*
			Make changes to the frame as necessary
			e.g.
			1. Change brightness/contrast of the image
			2. Smooth/Blur image
			3. Crop the image
			4. Rotate the image
			5. Draw shapes on the image
			*/
			// Convert frame to grayscale
			cvtColor(*StabCurrFrame, *StabCurrFrameGray, cv::COLOR_BGR2GRAY);
			// Convert frame to grayscale
			cvtColor(*StabPrevFrame, *StabPrevFrameGray, cv::COLOR_BGR2GRAY);

			StabCurrFeaturePoints.clear();
			StabPrevFeaturePoints.clear();

			// Detect features in previous frame
			goodFeaturesToTrack(*StabPrevFrameGray, StabPrevFeaturePoints, 200, 0.01, 15);

			
			// Calculate optical flow (i.e. track feature points)
			OptFlowStatus.clear();
			OptFlowErrors.clear();

			if (StabPrevFeaturePoints.size() > 0)
			{

				calcOpticalFlowPyrLK(*StabPrevFrameGray, *StabCurrFrameGray, StabPrevFeaturePoints, StabCurrFeaturePoints, OptFlowStatus, OptFlowErrors);

				// Filter only valid points
				auto prev_it = StabPrevFeaturePoints.begin();
				auto curr_it = StabCurrFeaturePoints.begin();
				for (size_t k = 0; k < OptFlowStatus.size(); k++)
				{
					if (OptFlowStatus[k])
					{
						prev_it++;
						curr_it++;
					}
					else
					{
						prev_it = StabPrevFeaturePoints.erase(prev_it);
						curr_it = StabCurrFeaturePoints.erase(curr_it);
					}
				}

				// Find transformation matrix
				*StabCurrTransformation = estimateRigidTransform(StabPrevFeaturePoints, StabCurrFeaturePoints, false);

				if (StabCurrTransformation->data == NULL) StabPrevTransformation->copyTo(*StabCurrTransformation);
				StabCurrTransformation->copyTo(*StabPrevTransformation);

				// Extract translation
				double dx = StabCurrTransformation->at<double>(0, 2);
				double dy = StabCurrTransformation->at<double>(1, 2);

				// Extract rotation angle
				double da = atan2(StabCurrTransformation->at<double>(1, 0), StabCurrTransformation->at<double>(0, 0));

				//===============================================================
				//INCLUDE BETWEEN THIS BLOCK THE SMOOTHING TRANSFORMATION CODE...
				//===============================================================
				// Store actual transformation 
				StabTransformations.push_back(TransformParam(dx, dy, da));

				ApplyMoveAverageFilter();

				//=============================================================
				//FINISH SMOOTHING TRANSFORMATION CODE...
				//=============================================================

				// Apply affine wrapping to the given frame
				warpAffine(*StabPrevFrame, *StabilizedFrame, *StabCurrTransformation, StabPrevFrame->size());

				// Fix Border - Scale image to remove black border artifact
				*FixRotationMatrix = getRotationMatrix2D(cv::Point2f(StabilizedFrame->cols / 2, StabilizedFrame->rows / 2), 0, 1.04);

				warpAffine(*StabilizedFrame, *FixFrameStabilized, *FixRotationMatrix, StabilizedFrame->size());

				//write the video frame to the file
				oVideoWriter.write(*FixFrameStabilized);
			}
			//Our Current Frame becomes the previous one
			StabPrevFrame = StabCurrFrame;

			FrameNumber++;

		}
	}

	StabCurrFrame->release();
	StabCurrFrameGray->release();
	StabPrevFrame->release();
	StabPrevFrameGray->release();
	StabCurrTransformation->release();
	StabPrevTransformation->release();
	StabilizedFrame->release();
	FixRotationMatrix->release();
	FixFrameStabilized->release();

	StabCurrFrame=nullptr;
	StabCurrFrameGray = nullptr;
	StabPrevFrame = nullptr;
	StabPrevFrameGray = nullptr;
	StabCurrTransformation = nullptr;
	StabPrevTransformation = nullptr;
	StabilizedFrame = nullptr;
	FixRotationMatrix = nullptr;
	FixFrameStabilized = nullptr;

	// Vector from previous and current feature points
	StabPrevFeaturePoints.clear();
	StabCurrFeaturePoints.clear();

	// Optical Flow results
	OptFlowStatus.clear();
	OptFlowErrors.clear();

	// Pre-define transformation-store array
	StabTransformations.clear();
	SmoothedTranformations.clear();

	// Transformation Trajectory
	StabTransfTrajectory.clear();
	StabSmoothTrajectory.clear();
}

void UIVRCameraManEditorSubsystem::SuperResolution()
{
	// 1-Initialize the initial parameters
	FString FramesFilePath = FPaths::ConvertRelativePathToFull(IVR_RecordingPath);
	bool    isSuccess = false;

	TArray<FString> Files;
	const TCHAR* extension = _T("*.mp4");

	IFileManager::Get().FindFilesRecursive(Files, *FramesFilePath, extension, true, false, false);

	for (int32 f = 0; f < Files.Num(); f++)
	{

		// Input and output video
		FString inputVideoName  = Files[f];
		//Cria Video de entrada com os frames gravados.
		cv::VideoCapture cap(TCHAR_TO_UTF8(*Files[f]));

		// if not success, exit program
		if (cap.isOpened() == false)
		{
			FString Message = "There is no recording for the file = " + Files[f] + "!";

			FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(Message));
			continue;
		}

		int frame_width = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_WIDTH)); //get the width of frames of the video
		int frame_height = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_HEIGHT)); //get the height of frames of the video
		int frames_per_second = static_cast<int>(cap.get(cv::CAP_PROP_FPS));

		cv::Size frame_size(frame_width, frame_height);

		//We just Use the Cap to read some recording parameters like FPS...
		cap.release();

		FString outputVideoName = FramesFilePath + "/" + "Super" + FString::FromInt(f) + "Output.mp4";

		// 2- Create an optical flow method
		Ptr<DenseOpticalFlowExt> optical_flow = createOptFlow(IVR_SuperOpticalFlow);

		//without an Optical Flow...No deal...
		if (optical_flow.empty())return;

		// 3- Create the superresolution method and set its parameters
		Ptr<cv::superres::SuperResolution> superRes;

		if (IVR_SuperOpticalFlow.Contains("Farneback") || IVR_SuperOpticalFlow.Contains("TVL1"))
		{
			//The method Follows if The Optical Flow have CUDA or Not...
			superRes = createSuperResolution_BTVL1();
		}
		else
		{
			//The other methods Have a CUDA Optical Flow Implementation, so we suppose this can be used only with them...
			//TODO: Make some experiments using a nonCUDA opt Flow, with CUDA SuperResolution Method...
			superRes = createSuperResolution_BTVL1_CUDA();
		}
		
		//Setup Parameters
		superRes->setOpticalFlow(optical_flow);
		superRes->setScale(IVR_SuperScale);
		superRes->setIterations(IVR_SuperIterations);
		superRes->setTemporalAreaRadius(IVR_SuperTemporalRadius);

		Ptr<FrameSource> frameSource;
		frameSource = createFrameSource_Video(TCHAR_TO_UTF8(*inputVideoName));

		superRes->setInput(frameSource);

		// Not use the first frame
		Mat frame;
		frameSource->nextFrame(frame);

		// 4- Processing the input video with the superresolution
		// Show the initial options
		VideoWriter writer;

		for (int i = 0;; ++i)
		{
			Mat result;

			// Calculate the processing time
			superRes->nextFrame(result);

			if (result.empty())break;
			// Save the result on output file
			if (!outputVideoName.IsEmpty())
			{
				if (!writer.isOpened())
					writer.open(TCHAR_TO_UTF8(*outputVideoName), VideoWriter::fourcc('3', 'I', 'V', 'D'), frames_per_second , result.size());
				writer << result;
			}
		}
		writer.release();
	}
}