// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhotoTargetComponent.generated.h"

USTRUCT(BlueprintType)
struct FPhotoTargetInfo {
	GENERATED_BODY();

	UPROPERTY()
		FName name;

	UPROPERTY()
		FColor color;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class COMBATGAME_API UPhotoTargetComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPhotoTargetComponent();

private:
	UPROPERTY(EditAnywhere)
		FPhotoTargetInfo targetInfo;

public:
	UFUNCTION(BlueprintCallable)
		FPhotoTargetInfo GetTargetInfo();
};