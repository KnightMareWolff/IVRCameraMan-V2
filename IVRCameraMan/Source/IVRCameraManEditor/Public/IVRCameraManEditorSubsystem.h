/************************************************************************/
/*Project              :IVR CameraMan - Unreal Plugin                   */
/*Creation Date/Author :William Wolff - 02/18/2021                      */
/*                                                                      */
/*Copyright (c) 2021 William Wolff. All rights reserved                 */
/************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "EditorSubsystem.h"
#include "IVR_SuperResolutionParameters.h"
#if WITH_OPENCV
#include "IVROpenCVHelper.h"
OPENCV_INCLUDES_START
#undef check // the check macro causes problems with opencv headers
#include "IVR_LowLevelSDK.h"
#include "opencv2/core/utility.hpp"
#include "opencv2/superres.hpp"
#include "opencv2/superres/optical_flow.hpp"
#include "opencv2/opencv_modules.hpp"
OPENCV_INCLUDES_END
#endif

#include "IVRCameraManEditorSubsystem.generated.h"

struct TransformParam
{
	TransformParam() {}
	TransformParam(double _dx, double _dy, double _da)
	{
		dx = _dx;
		dy = _dy;
		da = _da;
	}

	double dx;
	double dy;
	double da; // angle

	void getTransform(cv::Mat& T)
	{
		// Reconstruct transformation matrix accordingly to new values
		T.at<double>(0, 0) = cos(da);
		T.at<double>(0, 1) = -sin(da);
		T.at<double>(1, 0) = sin(da);
		T.at<double>(1, 1) = cos(da);

		T.at<double>(0, 2) = dx;
		T.at<double>(1, 2) = dy;
	}
};

struct Trajectory
{
	Trajectory() {}
	Trajectory(double _x, double _y, double _a) {
		x = _x;
		y = _y;
		a = _a;
	}

	double x;
	double y;
	double a; // angle
};

/**
 * 
 */
UCLASS()
class IVRCAMERAMANEDITOR_API UIVRCameraManEditorSubsystem : public UEditorSubsystem
{
	GENERATED_BODY()
public:

	// Triggered when starting the application
	void Initialize(FSubsystemCollectionBase& Collection) override;
	void Deinitialize() override;

	static void ReadCameraManDefaultValues(FString& pIVR_RecordingPath,
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
		                                   int& pIVR_MSecToWait);

	static void WriteCameraManDefaultValues(FString                   pIVR_RecordingPath,
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
		                                    int                       pIVR_MSecToWait);

	static void ReadExperimentalParametersValues(int32& pIVR_SuperScale,
		                                         int32& pIVR_SuperIterations,
		                                         int32& pIVR_SuperTemporalRadius,
		                                         FString& pIVR_SuperOpticalFlow);

	static void WriteExperimentalParametersValues(int32   pIVR_SuperScale,
		                                          int32   pIVR_SuperIterations,
		                                          int32   pIVR_SuperTemporalRadius,
		                                          FString pIVR_SuperOpticalFlow);


	static bool             IsEditorRunning;
	static FString          IVR_RecordingPath;
	static FString          IVR_VideoResolution;
	static int32            IVR_Width;
	static int32            IVR_Height;
	static FLinearColor     IVR_ClearColor;
	static float            IVR_Gamma;
	static bool             IVR_EnableTurboRecording;
	static float            IVR_TurboRate;
	static float            IVR_StabilizationArm;
	static float            IVR_LagDistance;
	static FString          IVR_CameraManName;
	static float            IVR_CameraVelocity;
	static float            IVR_BaseTurnRate;
	static float            IVR_BaseLookUpRate;
	static int32            IVR_SuperScale;
	static int32            IVR_SuperIterations;
	static int32            IVR_SuperTemporalRadius;
	static FString          IVR_SuperOpticalFlow;
	static bool             IVR_DebugRendering;
	static int              IVR_MSecToWait;

	//Video Stabilization Routines
	
	// Define variable for storing frames
	static cv::Mat* StabCurrFrame;
	static cv::Mat* StabCurrFrameGray;
	static cv::Mat* StabPrevFrame;
	static cv::Mat* StabPrevFrameGray;
	static cv::Mat* StabCurrTransformation;
	static cv::Mat* StabPrevTransformation;
	static cv::Mat* StabilizedFrame;
	static cv::Mat* FixRotationMatrix;
	static cv::Mat* FixFrameStabilized;

	// Vector from previous and current feature points
	static std::vector<cv::Point2f> StabPrevFeaturePoints;
	static std::vector<cv::Point2f> StabCurrFeaturePoints;

	// Optical Flow results
	static std::vector <uchar> OptFlowStatus;
	static std::vector <float> OptFlowErrors;

	// Pre-define transformation-store array
	static std::vector<TransformParam> StabTransformations;
	static std::vector<TransformParam> SmoothedTranformations;

	// Transformation Trajectory
	static std::vector <Trajectory> StabTransfTrajectory;
	static std::vector <Trajectory> StabSmoothTrajectory;

	static void StabilizeVideos();

	static bool ApplyMoveAverageFilter();
	static void smooth(std::vector<Trajectory>& trajectory, int radius);
	static void cumsum(std::vector<TransformParam>& transforms);

	//Video Super Resolution
	static void SuperResolution();
};
