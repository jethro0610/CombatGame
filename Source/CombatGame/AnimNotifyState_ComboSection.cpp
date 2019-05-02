#include "AnimNotifyState_ComboSection.h"
#include "CombatPawn.h"

void UAnimNotifyState_ComboSection::NotifyBegin(USkeletalMeshComponent* MeshComponent, UAnimSequenceBase* AnimationSequence, float TotalDuration) {
	ACombatPawn* owningPawn = Cast<ACombatPawn, AActor>(MeshComponent->GetOwner());
	
	if (owningPawn->IsValidLowLevel()) {
		owningPawn->OnAttack.RemoveDynamic(this, &UAnimNotifyState_ComboSection::OnPawnAttack);
		owningPawn->OnAttack.AddDynamic(this, &UAnimNotifyState_ComboSection::OnPawnAttack);

		owningPawn->EnableCombo();
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
	ACombatPawn* owningPawn = Cast<ACombatPawn, AActor>(MeshComponent->GetOwner());

	if (owningPawn->IsValidLowLevel()) {
		if(!hasAttacked)
			owningPawn->ResetCombo();
	}

	Received_NotifyEnd(MeshComponent, AnimationSequence);
}

void UAnimNotifyState_ComboSection::OnPawnAttack() {
	hasAttacked = true;
}