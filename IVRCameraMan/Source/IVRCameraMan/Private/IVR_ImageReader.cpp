/************************************************************************/
/*Project              :IVR CameraMan - Unreal Plugin                   */
/*Creation Date/Author :William Wolff - 03/06/2022                      */
/*                                                                      */
/*Copyright (c) 2021 William Wolff. All rights reserved                 */
/************************************************************************/
#include "IVR_ImageReader.h"

// Sets default values
AIVR_ImageReader::AIVR_ImageReader()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	IVR_ImageTexture = nullptr;
	IVR_ImageUpdateTextureRegion = nullptr;
	IVR_UpdatedTexture = nullptr;
	IVR_AlteredImageUpdateTextureRegion = nullptr;
}

// Called when the game starts or when spawned
void AIVR_ImageReader::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AIVR_ImageReader::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UTexture2D* AIVR_ImageReader::IVR_GetOriginalTexture(bool& IsValid)
{
	if (IVR_ImageTexture->IsValidLowLevel())
	{
		IsValid = true;
	}
	else
	{
		IsValid = false;
	}

	return IVR_ImageTexture;
}

UTexture2D* AIVR_ImageReader::IVR_GetUpdatedTexture(bool& IsValid)
{
	if (IVR_UpdatedTexture->IsValidLowLevel())
	{
		IsValid = true;
	}
	else
	{
		IsValid = false;
	}

	return IVR_UpdatedTexture;
}

bool AIVR_ImageReader::IVR_ReadImage(FString AbsolutePath)
{
	
	if (!IVR_ImageHandler)IVR_ImageHandler = new CIVRImageHandler();

	//Get the main Image Path
	const char* AbsPath = TCHAR_TO_UTF8(*AbsolutePath);

	bool Ret = IVR_ImageHandler->IVR_LoadImage(AbsPath);

	if (!Ret)return false;

	IVR_OriginalImage = IVR_ImageHandler->IVR_ImageData.clone();

	IVR_ImageSize = FVector2D(IVR_OriginalImage.cols, IVR_OriginalImage.rows);

	FRenderCommandFence InitializationFence;

	if (!IVR_ImageTexture)
	{
		IVR_ImageTexture = UTexture2D::CreateTransient(IVR_ImageSize.X, IVR_ImageSize.Y);
		IVR_ImageTexture->UpdateResource();
		IVR_ImageUpdateTextureRegion = new FUpdateTextureRegion2D(0, 0, 0, 0, IVR_ImageSize.X, IVR_ImageSize.Y);

		UpdateTexture();
	}
	else
	{
		UpdateTexture();
	}

	InitializationFence.Wait();

	//Inform the Blueprint - You can get the Texture!
	IVR_OnFinishReading();

	return Ret;
}

bool AIVR_ImageReader::IVR_ReadEffect(FString EffectName)
{

	if (!IVR_ImageHandler)IVR_ImageHandler = new CIVRImageHandler();

	//Get the main Image Path
	const char* AbsPath = TCHAR_TO_UTF8(*EffectName);

	bool Ret = IVR_ImageHandler->IVR_LoadEffect(AbsPath);

	if (!Ret)return false;

	return Ret;
}

bool AIVR_ImageReader::IVR_ReadRaw(TArray<FColor> RawBuffer)
{
	if (!IVR_ImageHandler)IVR_ImageHandler = new CIVRImageHandler();

	bool Ret = IVR_ImageHandler->IVR_LoadImageBUF((char*)RawBuffer.GetData(), RawBuffer.Num() * RawBuffer.GetTypeSize());

	if (!Ret)return false;

	IVR_OriginalImage = IVR_ImageHandler->IVR_ImageData.clone();

	IVR_ImageSize = FVector2D(IVR_OriginalImage.cols, IVR_OriginalImage.rows);

	FRenderCommandFence InitializationFence;

	if (!IVR_ImageTexture)
	{
		IVR_ImageTexture = UTexture2D::CreateTransient(IVR_ImageSize.X, IVR_ImageSize.Y);
		IVR_ImageTexture->UpdateResource();
		IVR_ImageUpdateTextureRegion = new FUpdateTextureRegion2D(0, 0, 0, 0, IVR_ImageSize.X, IVR_ImageSize.Y);

		UpdateTexture();
	}
	else
	{
		UpdateTexture();
	}

	InitializationFence.Wait();

	//Inform the Blueprint - You can get the Texture!
	IVR_OnFinishReading();

	return Ret;
}

bool AIVR_ImageReader::IVR_SaveImage(FString AbsolutePath)
{

	if (!IVR_ImageHandler)IVR_ImageHandler = new CIVRImageHandler();

	//Get the main Image Path
	const char* AbsPath = TCHAR_TO_UTF8(*AbsolutePath);

	bool Ret = IVR_ImageHandler->IVR_SaveImage(AbsPath);

	if (!Ret)return false;

	return Ret;
}

bool AIVR_ImageReader::IVR_SaveEffect(FString EffectName)
{

	if (!IVR_ImageHandler)IVR_ImageHandler = new CIVRImageHandler();

	//Get the main Image Path
	const char* AbsPath = TCHAR_TO_UTF8(*EffectName);

	bool Ret = IVR_ImageHandler->IVR_SaveEffect(AbsPath);

	if (!Ret)return false;

	return Ret;
}

bool AIVR_ImageReader::IVR_AddEffect(EEffectType EffectType)
{

	if (!IVR_ImageHandler)IVR_ImageHandler = new CIVRImageHandler();

	int32 EfTp = IVR_Image_Filter_Cartoon;
	
	switch (EffectType)
	{
	case EEffectType::Cartoon     : {EfTp = IVR_Image_Filter_Cartoon;   }break;
	case EEffectType::Binary      : {EfTp = IVR_Image_Filter_Binary;    }break;
	case EEffectType::Dilate      : {EfTp = IVR_Image_Filter_Dilate;    }break;
	case EEffectType::OldFilm     : {EfTp = IVR_Image_Filter_OldFilm;   }break;
	case EEffectType::PencilSketch: {EfTp = IVR_Image_Filter_PencilSkt; }break;
	case EEffectType::Sepia       : {EfTp = IVR_Image_Filter_Sepia;     }break;
	case EEffectType::Emboss      : {EfTp = IVR_Image_Filter_Emboss;    }break;
	case EEffectType::DuoTone     : {EfTp = IVR_Image_Filter_DuoTone;   }break;
	case EEffectType::Warm        : {EfTp = IVR_Image_Filter_Warm;      }break;
	case EEffectType::Cold        : {EfTp = IVR_Image_Filter_Cold;      }break;
	case EEffectType::Gotham      : {EfTp = IVR_Image_Filter_Gotham;    }break;
	case EEffectType::Sharpen     : {EfTp = IVR_Image_Filter_Sharpen;   }break;
	case EEffectType::Detail      : {EfTp = IVR_Image_Filter_Detail;    }break;
	case EEffectType::Invert      : {EfTp = IVR_Image_Filter_Invert;    }break;
	case EEffectType::Stylize     : {EfTp = IVR_Image_Filter_Stylize;   }break;
	case EEffectType::Ghost       : {EfTp = IVR_Image_Filter_Ghost;     }break;
	case EEffectType::None: {return false; }break;
	default: {return false; }break;
	}
	
	IVR_ImageHandler->IVR_AddFilter(EfTp);

	return true;
}

bool AIVR_ImageReader::IVR_SetEffectParameters(FIVR_EffectParameters EffectParameters)
{

	if (!IVR_ImageHandler)IVR_ImageHandler = new CIVRImageHandler();

	float pIVR_Treshold01     = EffectParameters.pIVR_Treshold01;
	float pIVR_Treshold02     = EffectParameters.pIVR_Treshold02;
	int   pIVR_ApertureSize   = EffectParameters.pIVR_ApertureSize;
	bool  pIVR_UseL2Gradient  = EffectParameters.pIVR_UseL2Gradient;
	int   pIVR_KernelSize     = EffectParameters.pIVR_KernelSize;
	float pIVR_SigmaS         = EffectParameters.pIVR_SigmaS;
	float pIVR_SigmaR         = EffectParameters.pIVR_SigmaR;
	int   pIVR_Iteractions    = EffectParameters.pIVR_Iteractions;
	int   pIVR_LightIntensity = EffectParameters.pIVR_LightIntensity;
	int   pIVR_ToneChannel01  = EffectParameters.pIVR_ToneChannel01;
	int   pIVR_ToneChannel02  = EffectParameters.pIVR_ToneChannel02;
	float pIVR_ExpValue       = EffectParameters.pIVR_ExpValue;
	float pIVR_ShadeFactor    = EffectParameters.pIVR_ShadeFactor;
	bool  pIVR_ColorOutput    = EffectParameters.pIVR_ColorOutput;
	int   pIVR_ToneR          = EffectParameters.pIVR_ToneR;
	int   pIVR_ToneG          = EffectParameters.pIVR_ToneG;
	int   pIVR_ToneB          = EffectParameters.pIVR_ToneB;
	int   pIVR_Transparency   = EffectParameters.pIVR_Transparency;

	IVR_ImageHandler->IVR_SetFilterParameters(pIVR_Treshold01,
		pIVR_Treshold02,
		pIVR_ApertureSize,
		pIVR_UseL2Gradient,
		pIVR_KernelSize,
		pIVR_SigmaS,
		pIVR_SigmaR,
		pIVR_Iteractions,
		pIVR_LightIntensity,
		pIVR_ToneChannel01,
		pIVR_ToneChannel02,
		pIVR_ExpValue,
		pIVR_ShadeFactor,
		pIVR_ColorOutput,
		pIVR_ToneR,
		pIVR_ToneG,
		pIVR_ToneB,
		pIVR_Transparency);

	return true;
}

void AIVR_ImageReader::IVR_ApplyEffects()
{
	if (!IVR_ImageHandler)IVR_ImageHandler = new CIVRImageHandler();

	IVR_ImageHandler->IVR_ApplyFilters();

	IVR_UpdatedImage = IVR_ImageHandler->IVR_ImageData.clone();

	IVR_UpdatedImageSize = FVector2D(IVR_UpdatedImage.cols, IVR_UpdatedImage.rows);

	FRenderCommandFence InitializationFence;

	if (!IVR_UpdatedTexture)
	{
		IVR_UpdatedTexture = UTexture2D::CreateTransient(IVR_UpdatedImageSize.X, IVR_UpdatedImageSize.Y);
		IVR_UpdatedTexture->UpdateResource();
		IVR_AlteredImageUpdateTextureRegion = new FUpdateTextureRegion2D(0, 0, 0, 0, IVR_UpdatedImageSize.X, IVR_UpdatedImageSize.Y);

		UpdateAlteredTexture();
	}
	else
	{
		UpdateAlteredTexture();
	}

	InitializationFence.Wait();

	//Inform the Blueprint - You can get the Texture!
	IVR_OnFinishUpdate();

	return;
}

void AIVR_ImageReader::IVR_ClearEffects()
{
	if (!IVR_ImageHandler)IVR_ImageHandler = new CIVRImageHandler();
	IVR_ImageHandler->IVR_CleanUpFilters();

	return;
}

void AIVR_ImageReader::UpdateTexture()
{
	IVR_ImageTextureData.Empty();
	IVR_ImageTextureData.Init(FColor(0, 0, 0, 255), IVR_ImageSize.X * IVR_ImageSize.Y);

	if (IVR_OriginalImage.data)
	{
		for (int y = 0; y < IVR_ImageSize.Y; y++)
		{
			for (int x = 0; x < IVR_ImageSize.X; x++)
			{
				int i = x + (y * IVR_ImageSize.X);
				IVR_ImageTextureData[i].B = IVR_OriginalImage.data[i * 4 + 0];
				IVR_ImageTextureData[i].G = IVR_OriginalImage.data[i * 4 + 1];
				IVR_ImageTextureData[i].R = IVR_OriginalImage.data[i * 4 + 2];
				IVR_ImageTextureData[i].A = IVR_OriginalImage.data[i * 4 + 3];
				
			}
		}

		UpdateTextureRegions(IVR_ImageTexture->GetResource()->GetTexture2DRHI(),
			(int32)0,
			(uint32)1,
			IVR_ImageUpdateTextureRegion,
			(uint32)(4 * IVR_ImageSize.X),
			(uint32)4,
			(uint8*)IVR_ImageTextureData.GetData(),
			false);
	}
}

void AIVR_ImageReader::UpdateAlteredTexture()
{
	IVR_UpdatedImageTextureData.Empty();
	IVR_UpdatedImageTextureData.Init(FColor(0, 0, 0, 255), IVR_UpdatedImageSize.X * IVR_UpdatedImageSize.Y);

	if (IVR_UpdatedImage.data)
	{
		for (int y = 0; y < IVR_UpdatedImageSize.Y; y++)
		{
			for (int x = 0; x < IVR_UpdatedImageSize.X; x++)
			{
				int i = x + (y * IVR_UpdatedImageSize.X);
				IVR_UpdatedImageTextureData[i].B = IVR_UpdatedImage.data[i * 4 + 0];
				IVR_UpdatedImageTextureData[i].G = IVR_UpdatedImage.data[i * 4 + 1];
				IVR_UpdatedImageTextureData[i].R = IVR_UpdatedImage.data[i * 4 + 2];
				IVR_UpdatedImageTextureData[i].A = IVR_UpdatedImage.data[i * 4 + 3];
			}
		}

		UpdateTextureRegions(IVR_UpdatedTexture->GetResource()->GetTexture2DRHI(),
			(int32)0,
			(uint32)1,
			IVR_AlteredImageUpdateTextureRegion,
			(uint32)(4 * IVR_UpdatedImageSize.X),
			(uint32)4,
			(uint8*)IVR_UpdatedImageTextureData.GetData(),
			false);
	}
}

/**
* This function updates the Texture Regions in the Graphics Hardware.
*
* Tip for the Day:
* Instead of call it normally like has the library does, here we will call RHI Directly scheduling a Job...Like happen with Rendering ;)
*/
void  AIVR_ImageReader::UpdateTextureRegions(FTexture2DRHIRef IVR_Texture,
	int32                   IVR_MipIndex,
	uint32                  IVR_NumRegions,
	FUpdateTextureRegion2D* IVR_Regions,
	int32                   IVR_SrcPitch,
	uint32                  IVR_SrcBpp,
	uint8* IVR_SrcData,
	bool                    IVR_FreeData)
{
	/**
	* This code works in version 4.26 without warnings.
	* in the 4.26 version we need use FRHITexture2D directly
	* the Lambda function as a parameter
	* the Mipmap checking was removed(since not exists anymore), and we are passing 0 to the MipMap Index
	* like already was made in the original UpdateTextureRegions (See the Library functions).
	* In the tests made it works fine, since we are not using this textures for rendering, so the base mip
	* is what we are looking for... ;)
	* If you need update the plugin to the 4.26 or greater use this code and comment the actual code to
	* work fine...
	*/
	if (IVR_Texture->IsValid())
	{
		struct FUpdateTextureRegionsData
		{
			FRHITexture2D* Texture2DResource;
			int32                   MipIndex;
			uint32                  NumRegions;
			FUpdateTextureRegion2D* Regions;
			int32                   SrcPitch;
			uint32                  SrcBpp;
			uint8* SrcData;
		};

		FUpdateTextureRegionsData* RegionData = new FUpdateTextureRegionsData;
		RegionData->Texture2DResource = IVR_Texture;
		RegionData->MipIndex = IVR_MipIndex;
		RegionData->NumRegions = IVR_NumRegions;
		RegionData->Regions = IVR_Regions;
		RegionData->SrcPitch = IVR_SrcPitch;
		RegionData->SrcBpp = IVR_SrcBpp;
		RegionData->SrcData = IVR_SrcData;

		ENQUEUE_RENDER_COMMAND(UpdateTextureRegionsData)(
			[RegionData, IVR_FreeData](FRHICommandListImmediate& RHICmdList)
		{
			for (uint32 RegionIndex = 0; RegionIndex < RegionData->NumRegions; ++RegionIndex)
			{
				RHIUpdateTexture2D(
					RegionData->Texture2DResource,
					RegionData->MipIndex,
					RegionData->Regions[RegionIndex],
					RegionData->SrcPitch,
					RegionData->SrcData
					+ RegionData->Regions[RegionIndex].SrcY * RegionData->SrcPitch
					+ RegionData->Regions[RegionIndex].SrcX * RegionData->SrcBpp
				);
			}
			if (IVR_FreeData)
			{
				FMemory::Free(RegionData->Regions);
				FMemory::Free(RegionData->SrcData);
			}
			delete RegionData;
		});
	}
}