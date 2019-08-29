#include "AnimNotifyState_SpawnHitbox.h"
#include "Components/SkeletalMeshComponent.h"
#include "Classes/Animation/AnimInstance.h"
#include "Engine.h"
#include "DrawDebugHelpers.h"

UAnimNotifyState_SpawnHitbox::UAnimNotifyState_SpawnHitbox() {

}

void UAnimNotifyState_SpawnHitbox::NotifyBegin(USkeletalMeshComponent* MeshComponent, UAnimSequenceBase* AnimationSequence, float TotalDuration) {
	// Set the owning pawn object
	if (owningPawn == nullptr)
		owningPawn = Cast<ACombatPawn,AActor>(MeshComponent->GetOwner());

	// Spawn the hitbox
	if(owningPawn != nullptr)
		spawnedHitbox = owningPawn->GetCombatComponent()->SpawnHitbox(MeshComponent, socket, offset, rotation, length, width, hitGroup, damage, horizontalKnockback, verticalKnockback);

	Received_NotifyBegin(MeshComponent, AnimationSequence, TotalDuration);
}

void UAnimNotifyState_SpawnHitbox::NotifyTick(USkeletalMeshComponent* MeshComponent, UAnimSequenceBase* AnimationSequence, float FrameDeltaTime) {
	currentMeshComponent = MeshComponent;
	currentAnimation = Cast<UAnimMontage, UAnimSequenceBase>(AnimationSequence);

	// Start drawing the hitbox for debug
	for (int i = 0; i < currentMeshComponent->GetAnimInstance()->ActiveAnimNotifyState.Num(); i++) {
		if (currentMeshComponent->GetAnimInstance()->ActiveAnimNotifyState[i].NotifyStateClass == this) {
			notifyStartTime = currentMeshComponent->GetAnimInstance()->ActiveAnimNotifyState[i].GetTriggerTime();
			notifyEndTime = currentMeshComponent->GetAnimInstance()->ActiveAnimNotifyState[i].GetEndTriggerTime();
		}
	}

	Received_NotifyTick(MeshComponent, AnimationSequence, FrameDeltaTime);
}

void UAnimNotifyState_SpawnHitbox::NotifyEnd(USkeletalMeshComponent* MeshComponent, UAnimSequenceBase* AnimationSequence) {
	// Remove the hitbox
	if (spawnedHitbox != nullptr) {
		spawnedHitbox->DeleteHitbox();
		spawnedHitbox = nullptr;
	}
	Received_NotifyEnd(MeshComponent, AnimationSequence);
}

void UAnimNotifyState_SpawnHitbox::Tick(float DeltaTime) {
	if (currentMeshComponent != nullptr && currentAnimation != nullptr && GIsEditor && !currentMeshComponent->GetWorld()->HasBegunPlay()) {
		float currentTime = currentMeshComponent->GetAnimInstance()->Montage_GetPosition(currentAnimation.Get());

		if (currentTime >= notifyStartTime && currentTime <= notifyEndTime) {
			// Draw the hitbox for debug along bone transform
			FTransform socketTransform = currentMeshComponent->GetSocketTransform(socket);
			FVector colliderPosition = socketTransform.TransformPosition(offset/socketTransform.GetScale3D());
			FQuat colliderRotation = socketTransform.TransformRotation(rotation.Quaternion());
			DrawDebugCapsule(currentMeshComponent->GetWorld(), colliderPosition, length, width, colliderRotation, FColor::Red);
		}
	}
}

bool UAnimNotifyState_SpawnHitbox::IsTickable() const {
	return true;
}

TStatId UAnimNotifyState_SpawnHitbox::GetStatId() const {
	return UObject::GetStatID();
}

bool UAnimNotifyState_SpawnHitbox::IsTickableInEditor() const {
	return true;
}

bool UAnimNotifyState_SpawnHitbox::IsTickableWhenPaused() const {
	return true;
}