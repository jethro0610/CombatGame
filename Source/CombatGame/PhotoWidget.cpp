// Fill out your copyright notice in the Description page of Project Settings.


#include "PhotoWidget.h"
#include "Engine.h"

void UPhotoWidget::OnUpdateImage() {
	SetBrushFromTexture(photograph->GetImage(), false);
}

void UPhotoWidget::SetPhotograph(UPhotograph* newPhotograph) {
	if (photograph != nullptr) {
		photograph->OnUpdateImage.RemoveDynamic(this, &UPhotoWidget::OnUpdateImage);
	}
	photograph = newPhotograph;
	photograph->OnUpdateImage.AddDynamic(this, &UPhotoWidget::OnUpdateImage);
	OnUpdateImage();
}