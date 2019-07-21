// Fill out your copyright notice in the Description page of Project Settings.


#include "Photograph.h"

UPhotograph::UPhotograph() {

}

void UPhotograph::UpdateImage(int newWidth, int newHeight, TArray<FColor> newImage) {
	delete image;
	imageWidth = newWidth;
	imageHeight = newHeight;
	image = UTexture2D::CreateTransient(imageWidth, imageHeight, PF_B8G8R8A8);
	image->MipGenSettings = TMGS_NoMipmaps;

	void* TextureData = image->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE); // Get texture location in memory and prepare to write
	FMemory::Memcpy(TextureData, newImage.GetData(), newImage.Num() * 4); // Copy image data to texture. Multiply array count by 4 for each color channel (RGBA)
	image->PlatformData->Mips[0].BulkData.Unlock(); // Finished writing to memory
	image->UpdateResource(); // Update texture
}

UTexture2D* UPhotograph::GetImage() {
	return image;
}

TArray<FColor> UPhotograph::GetImagePixelData() {
	const FColor* pixelByteArray = static_cast<const FColor*>(image->PlatformData->Mips[0].BulkData.LockReadOnly());
	TArray<FColor> pixelArray;
	for (int32 X = 0; X < image->GetSizeX(); X++)
	{
		for (int32 Y = 0; Y < image->GetSizeY(); Y++)
		{
			FColor pixel = pixelByteArray[Y * image->GetSizeX() + X];
			pixelArray.Add(pixel);
		}
	}
	image->PlatformData->Mips[0].BulkData.Unlock();
	return pixelArray;
}
