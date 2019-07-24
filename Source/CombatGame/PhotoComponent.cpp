// Fill out your copyright notice in the Description page of Project Settings.


#include "PhotoComponent.h"
#include "UObjectIterator.h"
#include "Engine.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UPhotoComponent::UPhotoComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPhotoComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UPhotoComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPhotoComponent::TakePhoto() {
	TArray<FColor> rawImage;
	FViewport* viewport = GEngine->GameViewport->Viewport;
	bool screenshotSuccesful = GetViewportScreenShot(viewport, rawImage);
	if (screenshotSuccesful) {
		UPhotograph* newPhotograph;
		newPhotograph = NewObject<UPhotograph>(GetOwner());
		newPhotograph->UpdateImage(viewport->GetSizeXY().X, viewport->GetSizeXY().Y, rawImage);
		photographs.Add(newPhotograph);
	}
	OnTakePhoto.Broadcast();
}

TArray<TWeakObjectPtr<UPhotoTargetComponent>> UPhotoComponent::GetTargetsInView() {
	TArray<TWeakObjectPtr<UPhotoTargetComponent>> photoTargets;
	for (TObjectIterator<UPhotoTargetComponent> Itr; Itr; ++Itr) {
		UPhotoTargetComponent* photoTarget = *Itr;
		if (photoTarget->GetOwner()->GetLastRenderTime() < 0.01f) {
			photoTargets.Add(photoTarget);
		}
	}
	return photoTargets;
}

UPhotograph* UPhotoComponent::GetPhotograph() {
	if (photographs.Num() > 0) {
		return photographs[photographs.Num() - 1];
	}
	else {
		return nullptr;
	}
}

