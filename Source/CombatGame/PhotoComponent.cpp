// Fill out your copyright notice in the Description page of Project Settings.


#include "PhotoComponent.h"
#include "UObjectIterator.h"
#include "UnrealClient.h"
#include "Engine.h"
#include "GameFramework/Actor.h"
#include "ImageUtils.h"

// Sets default values for this component's properties
UPhotoComponent::UPhotoComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	IImageWrapperModule &ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
	ImageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::PNG);
}


// Called when the game starts
void UPhotoComponent::BeginPlay()
{
	Super::BeginPlay();

	FViewport* viewport = GEngine->GameViewport->Viewport;
	int32 width = viewport->GetSizeXY().X;
	int32 height = viewport->GetSizeXY().Y;

	imageTexture = UTexture2D::CreateTransient(width, height, PF_B8G8R8A8);
	imageTexture->MipGenSettings = TMGS_NoMipmaps;
}


// Called every frame
void UPhotoComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPhotoComponent::TakePhoto() {
	TArray<FColor> uncompressedImage;
	FViewport* viewport = GEngine->GameViewport->Viewport;
	bool screenshotSuccesful = GetViewportScreenShot(viewport, uncompressedImage);
	if (screenshotSuccesful) {
		TArray<uint8> pngArray;
		int32 width = viewport->GetSizeXY().X;
		int32 height = viewport->GetSizeXY().Y;
		FImageUtils::CompressImageArray(width, height, uncompressedImage, pngArray);

		ImageWrapper->SetCompressed(pngArray.GetData(), pngArray.Num());
		const TArray<uint8>* decompressedImage = NULL;
		ImageWrapper->GetRaw(ERGBFormat::BGRA, 8, decompressedImage);
		void* TextureData = imageTexture->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
		FMemory::Memcpy(TextureData, decompressedImage->GetData(), decompressedImage->Num());
		imageTexture->PlatformData->Mips[0].BulkData.Unlock();
		imageTexture->UpdateResource();
	}
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

UTexture2D* UPhotoComponent::GetImageTexture() {
	return imageTexture;
}

