/************************************************************************/
/*Project              :IVR CameraMan - Unreal Plugin                   */
/*Creation Date/Author :William Wolff - 02/18/2021                      */
/*                                                                      */
/*Copyright (c) 2021 William Wolff. All rights reserved                 */
/************************************************************************/
#pragma once

#include "IVR_DefaultCameraParameters.h"
#include "IVR_SuperResolutionParameters.h"
#include "DetailLayoutBuilder.h"
#include "IDetailCustomization.h"
#include "IPropertyTypeCustomization.h"

/********************************************************************************/
/*The Customizer Field Class will be used to translate the information in the   */
/*Reflection System to a modifyable value during the tool functionality.        */
/*Sadly there is no easy way to do that becouse the data of the fields can be   */
/*managed thru IProperty Handles , but the identifications of the properies are */
/*stored inside of all different Object Properies.                              */
/*To make possible "link" the IProperty Handle with the correct Property we need*/
/*The Customyzer Field...                                                       */
/********************************************************************************/
class IVR_CustomizerField
{
public:
	//Field Control Attributes
	int                                  FieldOrder         ;
	FString                              FieldName          ;
	FProperty                          * FieldProperty      ;
	UObject                            * FieldOwner         ;
	void                               * FieldContainer     ;
	
	//Field Typing Attributes
	FString                     FieldType;
	bool                        IsNative;
	bool                        IsNumber;
	bool                        IsConditional;
	bool                        IsStruct;
	bool                        IsEnum;
	bool                        IsObject;
	bool                        IsChild;
	bool                        IsCustom;

	 IVR_CustomizerField() {}
	~IVR_CustomizerField() {}
};

/********************************************************************************/
/*The Detail Customizer Class will be used to extract all the information in the*/
/*Reflection System to build the Tool Screen.                                   */
/********************************************************************************/
class FIVR_DetailCustomizer : public	IDetailCustomization
{
public:
	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;

	static TSharedRef<IDetailCustomization> MakeInstance()
	{
		return MakeShareable(new FIVR_DetailCustomizer);
	}

	TWeakObjectPtr<class UIVR_DefaultCameraParameters> IVR_AssetClass;
	
	static TArray<TSharedPtr<IVR_CustomizerField> > IVR_PropertyHandleList;
};

class FIVR_PropertyCustomizer : public IPropertyTypeCustomization
{
public:
	
	static TSharedRef<IPropertyTypeCustomization> MakeInstance()
	{
		return MakeShareable(new FIVR_PropertyCustomizer);
	}
	
	UIVR_DefaultCameraParameters* IVR_AssetObject;
	
	virtual void CustomizeChildren(TSharedRef<IPropertyHandle>PropertyHandle, IDetailChildrenBuilder& ChildBuilder,	IPropertyTypeCustomizationUtils& CustomizationUtils) override;
	virtual void CustomizeHeader  (TSharedRef<IPropertyHandle>PropertyHandle, FDetailWidgetRow& HeaderRow,IPropertyTypeCustomizationUtils& CustomizationUtils) override;
};

/********************************************************************************/
/*The Detail Customizer Class will be used to extract all the information in the*/
/*Reflection System to build the Tool Screen.                                   */
/********************************************************************************/
class FIVR_SuperDetailCustomizer : public	IDetailCustomization
{
public:
	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;

	static TSharedRef<IDetailCustomization> MakeInstance()
	{
		return MakeShareable(new FIVR_SuperDetailCustomizer);
	}

	TWeakObjectPtr<class UIVR_SuperResolutionParameters> IVR_AssetClass;

	static TArray<TSharedPtr<IVR_CustomizerField> > IVR_PropertyHandleList;
};

class FIVR_SuperPropertyCustomizer : public IPropertyTypeCustomization
{
public:

	static TSharedRef<IPropertyTypeCustomization> MakeInstance()
	{
		return MakeShareable(new FIVR_SuperPropertyCustomizer);
	}

	UIVR_SuperResolutionParameters* IVR_AssetObject;

	virtual void CustomizeChildren(TSharedRef<IPropertyHandle>PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils) override;
	virtual void CustomizeHeader(TSharedRef<IPropertyHandle>PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils) override;
};