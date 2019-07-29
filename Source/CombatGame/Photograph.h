// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Texture2D.h"
#include "ImageWrapper/Public/IImageWrapper.h"
#include "ImageWrapper/Public/IImageWrapperModule.h"
#include "PhotoTargetComponent.h"
#include "Photograph.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUpdateImageDelegate);

UCLASS()
class COMBATGAME_API UPhotograph : public UObject
{
	GENERATED_BODY()
public:
	UPhotograph();

public:
	UPROPERTY(BlueprintAssignable)
		FUpdateImageDelegate OnUpdateImage;

private:
	int imageWidth;
	int imageHeight;

	UPROPERTY()
		UTexture2D* image;

	TArray<FPhotoTargetInfo> photoTargets;

public:
	UFUNCTION(BlueprintCallable)
		void UpdateImage(int newWidth, int newHeight, TArray<FColor> newImage);

	UFUNCTION(BlueprintCallable)
		void UpdateImageFromPNG(int pngWidth, int pngHeight, TArray<uint8> pngArray);

	UFUNCTION(BlueprintCallable)
		UTexture2D* GetImage();

	UFUNCTION(BlueprintCallable)
		TArray<FColor> GetImagePixelData();

	UFUNCTION(BlueprintCallable)
		TArray<uint8> GetImagePNG();

	UFUNCTION(BlueprintCallable)
		TArray<FPhotoTargetInfo> GetPhotoTargets();

	UFUNCTION(BlueprintCallable)
		void SetPhotoTargets(TArray<UPhotoTargetComponent*> newTargets);

	UFUNCTION(BlueprintCallable)
		void ExportImage(FString folder);
};
