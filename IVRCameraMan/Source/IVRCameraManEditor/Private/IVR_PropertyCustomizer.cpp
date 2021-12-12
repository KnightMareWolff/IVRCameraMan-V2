/************************************************************************/
/*Project              :IVR CameraMan - Unreal Plugin                   */
/*Creation Date/Author :William Wolff - 02/18/2021                      */
/*                                                                      */
/*Copyright (c) 2021 William Wolff. All rights reserved                 */
/************************************************************************/
#include "IVR_PropertyCustomizer.h"
#include "PropertyEditorModule.h"
#include "IVRCameraManEditor.h"
#include "IDetailsView.h"
#include "DetailLayoutBuilder.h"
#include "DetailCategoryBuilder.h"
//#include "Widgets/Colors/SColorPicker.h"
//#include "Widgets/SBoxPanel.h"
#include "DetailWidgetRow.h"

TArray<TSharedPtr<IVR_CustomizerField> > FIVR_DetailCustomizer::IVR_PropertyHandleList;
TArray<TSharedPtr<IVR_CustomizerField> > FIVR_SuperDetailCustomizer::IVR_PropertyHandleList;

void FIVR_DetailCustomizer::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	
	const TArray< TWeakObjectPtr<UObject> >& SelectedObjects =	DetailBuilder.GetDetailsView()->GetSelectedObjects();
	
	//Get the correct Object created at Initialization to extract the Property Values
	for (int32 ObjectIndex = 0; !IVR_AssetClass.IsValid() &&	ObjectIndex < SelectedObjects.Num(); ++ObjectIndex)
	{
		const TWeakObjectPtr<UObject>& CurrentObject =	SelectedObjects[ObjectIndex];
		if (CurrentObject.IsValid())
		{
			IVR_AssetClass =	Cast<UIVR_DefaultCameraParameters>(CurrentObject.Get());
		}
	}
	
	//Clear the Property Handle List
	IVR_PropertyHandleList.Empty();

	//Get all properties and fill all the handles in the order of declaration(This make possible list all the fields without worry about
	//if this Property are inside of a struct or not...
	for (TFieldIterator<FProperty> PropertyIterator(IVR_AssetClass->GetClass()); PropertyIterator; ++PropertyIterator)
	{
		//For each property the Unreal Reflection System provides all the information needed to build the refletion table
		FProperty* Property = *PropertyIterator;
		uint32     NumChildrens = 0;

		//Creates a new Field
		TSharedPtr<IVR_CustomizerField> tAssetField = MakeShareable(new IVR_CustomizerField());

		//Get the handle of this Field
		TSharedRef<IPropertyHandle> pHandle = DetailBuilder.GetProperty(Property->GetFName());

		//Fill the information to backtrace the field and values
		tAssetField.Get()->FieldProperty = Property;
		tAssetField.Get()->FieldOwner = IVR_AssetClass.Get();
		tAssetField.Get()->FieldName = Property->GetFName().ToString();
		tAssetField.Get()->FieldType = Property->GetCPPType();
		tAssetField.Get()->FieldContainer = NULL;

		//Initialize the Field Flags (This flags are more than enoth to stablish a good graph in future... ;)
		tAssetField.Get()->IsNative = false;
		tAssetField.Get()->IsNumber = false;
		tAssetField.Get()->IsConditional = false;
		tAssetField.Get()->IsStruct = false;
		tAssetField.Get()->IsEnum = false;
		tAssetField.Get()->IsObject = false;

		//Define if its a Native Declaration, a struct or an enumeration Field
		FString Workspace = Property->GetCPPTypeForwardDeclaration();
		if (Workspace.IsEmpty())
		{
			if (tAssetField.Get()->FieldType.Contains("int32")) { tAssetField.Get()->IsNumber = true; tAssetField.Get()->IsNative = true; }
			if (tAssetField.Get()->FieldType.Contains("long")) { tAssetField.Get()->IsNumber = true; tAssetField.Get()->IsNative = true; }
			if (tAssetField.Get()->FieldType.Contains("float")) { tAssetField.Get()->IsNumber = true; tAssetField.Get()->IsNative = true; }
			if (tAssetField.Get()->FieldType.Contains("double")) { tAssetField.Get()->IsNumber = true; tAssetField.Get()->IsNative = true; }
			if (tAssetField.Get()->FieldType.Contains("bool")) { tAssetField.Get()->IsConditional = true; tAssetField.Get()->IsNative = true; }
			if (tAssetField.Get()->FieldType.Contains("FString")) { tAssetField.Get()->IsNumber = false; tAssetField.Get()->IsNative = false; }
		}
		else if (Workspace.Contains("struct"))
		{
			tAssetField.Get()->IsStruct = true;
		}
		else if (Workspace.Contains("enum"))
		{
			tAssetField.Get()->IsEnum = true;
		}
		else if (Workspace.Contains("class"))
		{
			tAssetField.Get()->IsObject = true;
		}

		//If this property is a Object or a Struct , have Childrens that must be included on the Main List.
		pHandle->GetNumChildren(NumChildrens);
		if (NumChildrens > 0)
		{
			for (uint32 i = 0; i < NumChildrens; i++)
			{
				TSharedPtr<IPropertyHandle> pChildrenHandle = pHandle->GetChildHandle(i);
				FProperty*                  ChildProperty   = pChildrenHandle->GetProperty();

				TSharedPtr<IVR_CustomizerField> tChildrenField = MakeShareable(new IVR_CustomizerField());

				//Fill the information to backtrace the field and values
				tChildrenField.Get()->FieldProperty = ChildProperty;
				tChildrenField.Get()->FieldOwner = IVR_AssetClass.Get();
				tChildrenField.Get()->FieldName = ChildProperty->GetFName().ToString();
				tChildrenField.Get()->FieldType = ChildProperty->GetCPPType();

				//Get the Struct Container from the Father
				tChildrenField.Get()->FieldContainer = IVR_AssetClass.Get()->GetStructContainer(Property->GetFName());

				//Initialize the Field Flags (This flags are more than enoth to stablish a good graph in future... ;)
				tChildrenField.Get()->IsNative = false;
				tChildrenField.Get()->IsNumber = false;
				tChildrenField.Get()->IsConditional = false;
				tChildrenField.Get()->IsStruct = false;
				tChildrenField.Get()->IsEnum = false;
				tChildrenField.Get()->IsObject = false;

				//Define if its a Native Declaration, a struct or an enumeration Field
				Workspace = Property->GetCPPTypeForwardDeclaration();
				if (Workspace.IsEmpty())
				{
					if (tChildrenField.Get()->FieldType.Contains("int32")) { tChildrenField.Get()->IsNumber = true; tChildrenField.Get()->IsNative = true; }
					if (tChildrenField.Get()->FieldType.Contains("long")) { tChildrenField.Get()->IsNumber = true; tChildrenField.Get()->IsNative = true; }
					if (tChildrenField.Get()->FieldType.Contains("float")) { tChildrenField.Get()->IsNumber = true; tChildrenField.Get()->IsNative = true; }
					if (tChildrenField.Get()->FieldType.Contains("double")) { tChildrenField.Get()->IsNumber = true; tChildrenField.Get()->IsNative = true; }
					if (tChildrenField.Get()->FieldType.Contains("bool")) { tChildrenField.Get()->IsConditional = true; tChildrenField.Get()->IsNative = true; }
					if (tChildrenField.Get()->FieldType.Contains("FString")) { tChildrenField.Get()->IsNumber = false; tChildrenField.Get()->IsNative = false; }
				}
				else if (Workspace.Contains("struct"))
				{
					tChildrenField.Get()->IsStruct = true;
				}
				else if (Workspace.Contains("enum"))
				{
					tChildrenField.Get()->IsEnum = true;
				}
				else if (Workspace.Contains("class"))
				{
					tChildrenField.Get()->IsObject = true;
				}
				IVR_PropertyHandleList.Add(tChildrenField);
			}
		}
		else
		{
			IVR_PropertyHandleList.Add(tAssetField);
		}
	}
}

void FIVR_PropertyCustomizer::CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle,
	                                                           IDetailChildrenBuilder&	ChildBuilder, 
	                                                           IPropertyTypeCustomizationUtils&	CustomizationUtils)
{
	
}

void FIVR_PropertyCustomizer::CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle,
	                                                         FDetailWidgetRow& HeaderRow,
	                                                         IPropertyTypeCustomizationUtils& CustomizationUtils)
{

}

void FIVR_SuperDetailCustomizer::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{

	const TArray< TWeakObjectPtr<UObject> >& SelectedObjects = DetailBuilder.GetDetailsView()->GetSelectedObjects();

	//Get the correct Object created at Initialization to extract the Property Values
	for (int32 ObjectIndex = 0; !IVR_AssetClass.IsValid() && ObjectIndex < SelectedObjects.Num(); ++ObjectIndex)
	{
		const TWeakObjectPtr<UObject>& CurrentObject = SelectedObjects[ObjectIndex];
		if (CurrentObject.IsValid())
		{
			IVR_AssetClass = Cast<UIVR_SuperResolutionParameters>(CurrentObject.Get());
		}
	}

	//Clear the Property Handle List
	IVR_PropertyHandleList.Empty();

	//Get all properties and fill all the handles in the order of declaration(This make possible list all the fields without worry about
	//if this Property are inside of a struct or not...
	for (TFieldIterator<FProperty> PropertyIterator(IVR_AssetClass->GetClass()); PropertyIterator; ++PropertyIterator)
	{
		//For each property the Unreal Reflection System provides all the information needed to build the refletion table
		FProperty* Property = *PropertyIterator;
		uint32     NumChildrens = 0;

		//Creates a new Field
		TSharedPtr<IVR_CustomizerField> tAssetField = MakeShareable(new IVR_CustomizerField());

		//Get the handle of this Field
		TSharedRef<IPropertyHandle> pHandle = DetailBuilder.GetProperty(Property->GetFName());

		//Fill the information to backtrace the field and values
		tAssetField.Get()->FieldProperty = Property;
		tAssetField.Get()->FieldOwner = IVR_AssetClass.Get();
		tAssetField.Get()->FieldName = Property->GetFName().ToString();
		tAssetField.Get()->FieldType = Property->GetCPPType();
		tAssetField.Get()->FieldContainer = NULL;

		//Initialize the Field Flags (This flags are more than enoth to stablish a good graph in future... ;)
		tAssetField.Get()->IsNative = false;
		tAssetField.Get()->IsNumber = false;
		tAssetField.Get()->IsConditional = false;
		tAssetField.Get()->IsStruct = false;
		tAssetField.Get()->IsEnum = false;
		tAssetField.Get()->IsObject = false;

		//Define if its a Native Declaration, a struct or an enumeration Field
		FString Workspace = Property->GetCPPTypeForwardDeclaration();
		if (Workspace.IsEmpty())
		{
			if (tAssetField.Get()->FieldType.Contains("int32")) { tAssetField.Get()->IsNumber = true; tAssetField.Get()->IsNative = true; }
			if (tAssetField.Get()->FieldType.Contains("long")) { tAssetField.Get()->IsNumber = true; tAssetField.Get()->IsNative = true; }
			if (tAssetField.Get()->FieldType.Contains("float")) { tAssetField.Get()->IsNumber = true; tAssetField.Get()->IsNative = true; }
			if (tAssetField.Get()->FieldType.Contains("double")) { tAssetField.Get()->IsNumber = true; tAssetField.Get()->IsNative = true; }
			if (tAssetField.Get()->FieldType.Contains("bool")) { tAssetField.Get()->IsConditional = true; tAssetField.Get()->IsNative = true; }
			if (tAssetField.Get()->FieldType.Contains("FString")) { tAssetField.Get()->IsNumber = false; tAssetField.Get()->IsNative = false; }
		}
		else if (Workspace.Contains("struct"))
		{
			tAssetField.Get()->IsStruct = true;
		}
		else if (Workspace.Contains("enum"))
		{
			tAssetField.Get()->IsEnum = true;
		}
		else if (Workspace.Contains("class"))
		{
			tAssetField.Get()->IsObject = true;
		}

		//If this property is a Object or a Struct , have Childrens that must be included on the Main List.
		pHandle->GetNumChildren(NumChildrens);
		if (NumChildrens > 0)
		{
			for (uint32 i = 0; i < NumChildrens; i++)
			{
				TSharedPtr<IPropertyHandle> pChildrenHandle = pHandle->GetChildHandle(i);
				FProperty* ChildProperty = pChildrenHandle->GetProperty();

				TSharedPtr<IVR_CustomizerField> tChildrenField = MakeShareable(new IVR_CustomizerField());

				//Fill the information to backtrace the field and values
				tChildrenField.Get()->FieldProperty = ChildProperty;
				tChildrenField.Get()->FieldOwner = IVR_AssetClass.Get();
				tChildrenField.Get()->FieldName = ChildProperty->GetFName().ToString();
				tChildrenField.Get()->FieldType = ChildProperty->GetCPPType();

				//Get the Struct Container from the Father
				tChildrenField.Get()->FieldContainer = IVR_AssetClass.Get()->GetStructContainer(Property->GetFName());

				//Initialize the Field Flags (This flags are more than enoth to stablish a good graph in future... ;)
				tChildrenField.Get()->IsNative = false;
				tChildrenField.Get()->IsNumber = false;
				tChildrenField.Get()->IsConditional = false;
				tChildrenField.Get()->IsStruct = false;
				tChildrenField.Get()->IsEnum = false;
				tChildrenField.Get()->IsObject = false;

				//Define if its a Native Declaration, a struct or an enumeration Field
				Workspace = Property->GetCPPTypeForwardDeclaration();
				if (Workspace.IsEmpty())
				{
					if (tChildrenField.Get()->FieldType.Contains("int32")) { tChildrenField.Get()->IsNumber = true; tChildrenField.Get()->IsNative = true; }
					if (tChildrenField.Get()->FieldType.Contains("long")) { tChildrenField.Get()->IsNumber = true; tChildrenField.Get()->IsNative = true; }
					if (tChildrenField.Get()->FieldType.Contains("float")) { tChildrenField.Get()->IsNumber = true; tChildrenField.Get()->IsNative = true; }
					if (tChildrenField.Get()->FieldType.Contains("double")) { tChildrenField.Get()->IsNumber = true; tChildrenField.Get()->IsNative = true; }
					if (tChildrenField.Get()->FieldType.Contains("bool")) { tChildrenField.Get()->IsConditional = true; tChildrenField.Get()->IsNative = true; }
					if (tChildrenField.Get()->FieldType.Contains("FString")) { tChildrenField.Get()->IsNumber = false; tChildrenField.Get()->IsNative = false; }
				}
				else if (Workspace.Contains("struct"))
				{
					tChildrenField.Get()->IsStruct = true;
				}
				else if (Workspace.Contains("enum"))
				{
					tChildrenField.Get()->IsEnum = true;
				}
				else if (Workspace.Contains("class"))
				{
					tChildrenField.Get()->IsObject = true;
				}
				IVR_PropertyHandleList.Add(tChildrenField);
			}
		}
		else
		{
			IVR_PropertyHandleList.Add(tAssetField);
		}
	}
}

void FIVR_SuperPropertyCustomizer::CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle,
	IDetailChildrenBuilder& ChildBuilder,
	IPropertyTypeCustomizationUtils& CustomizationUtils)
{

}

void FIVR_SuperPropertyCustomizer::CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle,
	FDetailWidgetRow& HeaderRow,
	IPropertyTypeCustomizationUtils& CustomizationUtils)
{

}
