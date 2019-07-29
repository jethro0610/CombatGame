// Fill out your copyright notice in the Description page of Project Settings.


#include "Photograph.h"
#include "Engine.h"
#include "ImageUtils.h"

UPhotograph::UPhotograph() {

}

void UPhotograph::UpdateImage(int newWidth, int newHeight, TArray<FColor> newImage) {
	if (image != nullptr) {
		image->ConditionalBeginDestroy();
		image = nullptr;
	}
	imageWidth = newWidth;
	imageHeight = newHeight;
	image = UTexture2D::CreateTransient(imageWidth, imageHeight, PF_B8G8R8A8);
	image->MipGenSettings = TMGS_NoMipmaps;

	void* TextureData = image->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE); // Get texture location in memory and prepare to write
	FMemory::Memcpy(TextureData, newImage.GetData(), newImage.Num() * 4); // Copy image data to texture. Multiply array count by 4 for each color channel (RGBA)
	image->PlatformData->Mips[0].BulkData.Unlock(); // Finished writing to memory
	image->UpdateResource(); // Update texture

	OnUpdateImage.Broadcast();
}

void UPhotograph::UpdateImageFromPNG(int pngWidth, int pngHeight, TArray<uint8> pngArray) {
	
	if (image != nullptr) {
		image->ConditionalBeginDestroy();
		image = nullptr;
	}
	
	imageWidth = pngWidth;
	imageHeight = pngHeight;
	image = UTexture2D::CreateTransient(imageWidth, imageHeight, PF_B8G8R8A8);
	image->MipGenSettings = TMGS_NoMipmaps;

	IImageWrapperModule &ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
	TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::PNG);

	ImageWrapper->SetCompressed(pngArray.GetData(), pngArray.Num());
	const TArray<uint8>* decompressedImage = NULL;
	ImageWrapper->GetRaw(ERGBFormat::BGRA, 8, decompressedImage);

	void* TextureData = image->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE); // Get texture location in memory and prepare to write
	FMemory::Memcpy(TextureData, decompressedImage->GetData(), decompressedImage->Num()); // Copy image data to texture.
	image->PlatformData->Mips[0].BulkData.Unlock(); // Finished writing to memory
	image->UpdateResource(); // Update texture

	OnUpdateImage.Broadcast();
}

UTexture2D* UPhotograph::GetImage() {
	return image;
}

TArray<FColor> UPhotograph::GetImagePixelData() {
	const FColor* pixelByteArray = static_cast<const FColor*>(image->PlatformData->Mips[0].BulkData.LockReadOnly());
	TArray<FColor> pixelArray;
	for (int i = 0; i < imageWidth * imageHeight; i++)
	{
		FColor pixel = pixelByteArray[i];
		pixelArray.Add(pixel);
	}
	image->PlatformData->Mips[0].BulkData.Unlock();

	return pixelArray;
}

TArray<uint8> UPhotograph::GetImagePNG() {
	TArray<uint8> pngArray;
	FImageUtils::CompressImageArray(imageWidth, imageHeight, GetImagePixelData(), pngArray);
	return pngArray;
}

TArray<FPhotoTargetInfo> UPhotograph::GetPhotoTargets() {
	return photoTargets;
}

void UPhotograph::SetPhotoTargets(TArray<UPhotoTargetComponent*> newTargets) {
	photoTargets.Empty();
	for (int i = 0; i < newTargets.Num(); i++) {
		photoTargets.Add(newTargets[i]->GetTargetInfo());
	}
}

void UPhotograph::ExportImage(FString folder) {
	FString filePath = folder + "/" + FDateTime::Now().ToString() + ".png";
	FFileHelper::SaveArrayToFile(GetImagePNG(), *filePath);
}
