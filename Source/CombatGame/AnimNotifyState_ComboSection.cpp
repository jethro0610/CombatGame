#include "AnimNotifyState_ComboSection.h"

void UAnimNotifyState_ComboSection::NotifyBegin(USkeletalMeshComponent* MeshComponent, UAnimSequenceBase* AnimationSequence, float TotalDuration) {
	// Set the owning player object
	if(owningPlayer == nullptr)
		owningPlayer = Cast<APlayerPawn, AActor>(MeshComponent->GetOwner());
	
	if (owningPlayer != nullptr) {
		// Bind delegate
		owningPlayer->OnAttack.RemoveDynamic(this, &UAnimNotifyState_ComboSection::OnPawnAttack);
		owningPlayer->OnAttack.AddDynamic(this, &UAnimNotifyState_ComboSection::OnPawnAttack);
		owningPlayer->EnableCombo();
	}
	isInitialTick = true;
	hasAttacked = false;
	Received_NotifyBegin(MeshComponent, AnimationSequence, TotalDuration);
}

void UAnimNotifyState_ComboSection::NotifyTick(USkeletalMeshComponent* MeshComponent, UAnimSequenceBase* AnimationSequence, float TotalDuration) {
	if (isInitialTick) {
		hasAttacked = false;
		isInitialTick = false;
	}
	Received_NotifyBegin(MeshComponent, AnimationSequence, TotalDuration);
}

void UAnimNotifyState_ComboSection::NotifyEnd(USkeletalMeshComponent* MeshComponent, UAnimSequenceBase* AnimationSequence) {
	if (owningPlayer != nullptr) {
		if(!hasAttacked)
			owningPlayer->ResetCombo();
	}

	Received_NotifyEnd(MeshComponent, AnimationSequence);
}

void UAnimNotifyState_ComboSection::OnPawnAttack() {
	hasAttacked = true;
}