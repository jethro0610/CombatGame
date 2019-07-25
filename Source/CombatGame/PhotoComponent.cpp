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
	GetTargetsInView();
}

float UPhotoComponent::GetActorRenderTimeDifference(AActor* actorToCheck, float currentTime) {
	float actorLastRenderTime = actorToCheck->GetLastRenderTime();

	return currentTime - actorLastRenderTime;
}

bool UPhotoComponent::ActorHasBeenRendered(AActor* actorToCheck, float currentTime) {
	return GetActorRenderTimeDifference(actorToCheck, currentTime) < 0.1f;
}

bool UPhotoComponent::ActorIsWithinViewport(AActor* actorToCheck) {
	FViewport* viewport = GEngine->GameViewport->Viewport;
	FVector2D location;
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->ProjectWorldLocationToScreen(actorToCheck->GetActorLocation(), location, false);

	if (FMath::IsWithin(location.X, 0.0f, (float)viewport->GetSizeXY().X) && FMath::IsWithin(location.Y, 0.0f, (float)viewport->GetSizeXY().Y)) {
		return true;
	}
	else {
		return false;
	}
}

TArray<UPhotoTargetComponent*> UPhotoComponent::GetTargetsInView() {
	TArray<UPhotoTargetComponent*> photoTargets;
	FViewport* viewport = GEngine->GameViewport->Viewport;
	float currentTime = UGameplayStatics::GetTimeSeconds(GetWorld());
	for (TObjectIterator<UPhotoTargetComponent> Itr; Itr; ++Itr) {
		UPhotoTargetComponent* photoTarget = *Itr;

		if (photoTarget->GetWorld() != GetWorld())
			continue;
		if (ActorIsWithinViewport(photoTarget->GetOwner())) {
			if (ActorHasBeenRendered(photoTarget->GetOwner(), currentTime)) {
				photoTargets.Add(photoTarget);
			}
		}
	}
	return photoTargets;
}

TArray<UPhotograph*> UPhotoComponent::GetPhotographs() {
	return photographs;
}

