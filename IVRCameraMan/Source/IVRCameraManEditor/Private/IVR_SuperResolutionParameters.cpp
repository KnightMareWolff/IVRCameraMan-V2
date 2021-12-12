/************************************************************************/
/*Project              :IVR CameraMan - Unreal Plugin                   */
/*Creation Date/Author :William Wolff - 02/18/2021                      */
/*                                                                      */
/*Copyright (c) 2021 William Wolff. All rights reserved                 */
/************************************************************************/


#include "IVR_SuperResolutionParameters.h"

void* UIVR_SuperResolutionParameters::GetStructContainer(FName pPropertyName)
{
	FString tCheckedName = pPropertyName.ToString();
	void* StructPtr;

	if (tCheckedName.Contains("IVR_"))
	{
		// get a pointer to the struct instance
		FProperty* StructProp = StaticClass()->FindPropertyByName(pPropertyName);

		StructPtr = StructProp->ContainerPtrToValuePtr<void>(this);
	}
	else
	{
		StructPtr = NULL;
	}

	return StructPtr;
}

UObject* UIVR_SuperResolutionParameters::GetStructObject(FName pPropertyName)
{
	FString  tCheckedName = pPropertyName.ToString();
	UObject* ObjectPtr;

	if (tCheckedName.Contains("IVR_"))
	{
		// get a pointer to the struct instance
		FProperty* StructProp = StaticClass()->FindPropertyByName(pPropertyName);

		ObjectPtr = StructProp->ContainerPtrToValuePtr<UObject>(this);
	}
	else
	{
		ObjectPtr = NULL;
	}

	return ObjectPtr;
}