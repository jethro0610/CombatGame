// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Texture2D.h"
#include "Photograph.generated.h"

/**
 * 
 */
UCLASS()
class COMBATGAME_API UPhotograph : public UObject
{
	GENERATED_BODY()
public:
	UPhotograph();

private:
	int imageWidth;
	int imageHeight;

	UPROPERTY()
		UTexture2D* image;

public:
	UFUNCTION(BlueprintCallable)
		void UpdateImage(int newWidth, int newHeight, TArray<FColor> newImage);

	UFUNCTION(BlueprintCallable)
		UTexture2D* GetImage();

	UFUNCTION(BlueprintCallable)
		TArray<FColor> GetImagePixelData();
};
