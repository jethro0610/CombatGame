// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Image.h"
#include "Photograph.h"
#include "Styling/SlateBrush.h"
#include "PhotoWidget.generated.h"

/**
 * 
 */
UCLASS()
class COMBATGAME_API UPhotoWidget : public UImage
{
	GENERATED_BODY()

private:
	TWeakObjectPtr<UPhotograph> photograph;

	UFUNCTION()
		void OnUpdateImage();

public:
	UFUNCTION(BlueprintCallable)
		void SetPhotograph(UPhotograph* newPhotograph);
};
