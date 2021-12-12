/************************************************************************/
/*Project              :IVR CameraMan - Unreal Plugin                   */
/*Creation Date/Author :William Wolff - 02/18/2021                      */
/*                                                                      */
/*Copyright (c) 2021 William Wolff. All rights reserved                 */
/************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "IVR_DefaultCameraParameters.h"
#include "IVR_SuperResolutionParameters.h"
#include "IVR_PropertyCustomizer.h"
#include "IVRCameraManEditorStyle.h"
#include "IVR_WidgetBuilder.generated.h"

/**
 * 
 */
UCLASS()
class IVRCAMERAMANEDITOR_API UIVR_WidgetBuilder : public UObject
{
	GENERATED_BODY()

public:

	TSharedPtr<IDetailsView> pWidgetView;
	TSharedPtr<IDetailsView> pSuperResWidget;

	TSharedRef<IDetailsView> GetRef() { return pWidgetView.ToSharedRef(); }
	TSharedRef<IDetailsView> GetSuperRef(){ return pSuperResWidget.ToSharedRef(); }

	//Initialize and build widget data.
	void BuildWidget();
	
	//Used to handle the changes happening in the controls
	void OnFinishedChangingProperties(const FPropertyChangedEvent& PropertyChangedEvent);
	
	// Holds project configuration data
	UIVR_DefaultCameraParameters  * IVR_ConfigurationDataSource;
	UIVR_SuperResolutionParameters* IVR_SuperResDataSource;
	
};
