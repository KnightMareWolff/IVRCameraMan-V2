/************************************************************************/
/*Project              :IVR CameraMan - Unreal Plugin                   */
/*Creation Date/Author :William Wolff - 02/06/2022                      */
/*                                                                      */
/*Copyright (c) 2022 William Wolff. All rights reserved                 */
/************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IVR_FunctionLibrary.h" //The Function Library already have the OpenCV Includes
#include "IVR_ImageReader.generated.h"

UCLASS()
class IVRCAMERAMAN_API AIVR_ImageReader : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AIVR_ImageReader();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Read/Load Functions
	UFUNCTION(Category = "CameraMan|Utils|ImageReader", BlueprintCallable)
	bool IVR_ReadImage(FString AbsolutePath);
	UFUNCTION(Category = "CameraMan|Utils|ImageReader", BlueprintCallable)
    bool IVR_ReadRaw  (TArray<FColor> RawBuffer);
	UFUNCTION(Category = "CameraMan|Utils|ImageReader", BlueprintCallable)
	bool IVR_ReadEffect(FString EffectName);
	
	//Save Functions
	UFUNCTION(Category = "CameraMan|Utils|ImageReader", BlueprintCallable)
	bool IVR_SaveImage(FString  AbsolutePath);
	UFUNCTION(Category = "CameraMan|Utils|ImageReader", BlueprintCallable)
	bool IVR_SaveEffect(FString EffectName);

	//Effect Functions
	UFUNCTION(Category = "CameraMan|Utils|ImageReader", BlueprintCallable)
	bool IVR_AddEffect(EEffectType EffectType);
	UFUNCTION(Category = "CameraMan|Utils|ImageReader", BlueprintCallable)
	bool IVR_SetEffectParameters(FIVR_EffectParameters EffectParameters);
	UFUNCTION(Category = "CameraMan|Utils|ImageReader", BlueprintCallable)
	void IVR_ApplyEffects();
	UFUNCTION(Category = "CameraMan|Utils|ImageReader", BlueprintCallable)
	void IVR_ClearEffects();

	//Texture Getters
	UFUNCTION(Category = "CameraMan|Utils|ImageReader", BlueprintCallable)
	UTexture2D* IVR_GetOriginalTexture(bool& IsValid);
	UFUNCTION(Category = "CameraMan|Utils|ImageReader", BlueprintCallable)
	UTexture2D* IVR_GetUpdatedTexture(bool& IsValid);
	
	//Blueprint Event called everytime a image finish all the reading operation
	UFUNCTION(Category = "CameraMan|Utils|ImageReader", BlueprintImplementableEvent)
	void IVR_OnFinishReading();

	//Blueprint Event called everytime a image finish all the reading operation
	UFUNCTION(Category = "CameraMan|Utils|ImageReader", BlueprintImplementableEvent)
	void IVR_OnFinishUpdate();

	void UpdateTexture();
	void UpdateAlteredTexture();

	CIVRImageHandler* IVR_ImageHandler;
	Mat               IVR_OriginalImage;
	Mat               IVR_UpdatedImage;

	//Informs the Size of the video comming from the Hardware(Usefull to rescale the Image after a Processing)
	UPROPERTY(BlueprintReadWrite, Category = "CameraMan|Utils|ImageReader")
	FVector2D IVR_ImageSize;
	
	UPROPERTY(BlueprintReadOnly, Category = "CameraMan|Utils|ImageReader")
	FVector2D IVR_UpdatedImageSize;

	//Handle the texture created by Unreal inside of Graphics Hardware(Default Vulkan)
	UPROPERTY(BlueprintReadOnly, Category = "CameraMan|Utils|ImageReader")
	UTexture2D* IVR_ImageTexture;

	//Handle the texture created by Unreal inside of Graphics Hardware(Default Vulkan)
	UPROPERTY(BlueprintReadOnly, Category = "CameraMan|Utils|ImageReader")
	UTexture2D* IVR_UpdatedTexture;

	//Handle the Pixel Data to be updated inside of the Texture Regions(Warning about OpenCV PixelFormats)
	UPROPERTY(BlueprintReadOnly, Category = "CameraMan|Utils|ImageReader")
	TArray<FColor> IVR_ImageTextureData;

	//Handle the Pixel Data to be updated inside of the Texture Regions(Warning about OpenCV PixelFormats)
	UPROPERTY(BlueprintReadOnly, Category = "CameraMan|Utils|ImageReader")
	TArray<FColor> IVR_UpdatedImageTextureData;

	void UpdateTextureRegions(FTexture2DRHIRef IVR_Texture,
		int32                   IVR_MipIndex,
		uint32                  IVR_NumRegions,
		FUpdateTextureRegion2D* IVR_Regions,
		int32                   IVR_SrcPitch,
		uint32                  IVR_SrcBpp,
		uint8* IVR_SrcData,
		bool                    IVR_FreeData);

	FUpdateTextureRegion2D* IVR_ImageUpdateTextureRegion;
	FUpdateTextureRegion2D* IVR_AlteredImageUpdateTextureRegion;

};
