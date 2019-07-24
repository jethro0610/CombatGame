// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhotoTargetComponent.h"
#include "Engine/Texture2D.h"
#include "Photograph.h"
#include "PhotoComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTakePhotoDelegate);
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COMBATGAME_API UPhotoComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPhotoComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	TArray<UPhotoTargetComponent*> GetTargetsInView();

	UPROPERTY(BlueprintAssignable)
		FTakePhotoDelegate OnTakePhoto;

private:
	UPROPERTY()	
		TArray <UPhotograph*> photographs;

public:
	UFUNCTION(BlueprintCallable)
		void TakePhoto();

	UFUNCTION(BlueprintCallable)
		UPhotograph* GetPhotograph();
};
