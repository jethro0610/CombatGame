#include "AnimNotifyState_SpawnHitbox.h"
#include "Components/SkeletalMeshComponent.h"
#include "Classes/Animation/AnimInstance.h"
#include "Engine.h"
#include "DrawDebugHelpers.h"

UAnimNotifyState_SpawnHitbox::UAnimNotifyState_SpawnHitbox() {

}

void UAnimNotifyState_SpawnHitbox::NotifyBegin(USkeletalMeshComponent* MeshComponent, UAnimSequenceBase* AnimationSequence, float TotalDuration) {
	if (!owningPawn->IsValidLowLevel())
		owningPawn = Cast<ACombatPawn,AActor>(MeshComponent->GetOwner());

	if(owningPawn->IsValidLowLevel())
		spawnedHitbox = owningPawn->GetCombatComponent()->SpawnHitbox(MeshComponent, socket, offset, rotation, length, width, hitGroup, damage, horizontalKnockback, verticalKnockback);

	Received_NotifyBegin(MeshComponent, AnimationSequence, TotalDuration);
}

void UAnimNotifyState_SpawnHitbox::NotifyTick(USkeletalMeshComponent* MeshComponent, UAnimSequenceBase* AnimationSequence, float FrameDeltaTime) {
	currentMeshComponent = MeshComponent;
	currentAnimation = Cast<UAnimMontage, UAnimSequenceBase>(AnimationSequence);

	notifyStartTime = currentMeshComponent->GetAnimInstance()->ActiveAnimNotifyState[0].GetTriggerTime();
	notifyEndTime = currentMeshComponent->GetAnimInstance()->ActiveAnimNotifyState[0].GetEndTriggerTime();

	Received_NotifyTick(MeshComponent, AnimationSequence, FrameDeltaTime);
}

void UAnimNotifyState_SpawnHitbox::NotifyEnd(USkeletalMeshComponent* MeshComponent, UAnimSequenceBase* AnimationSequence) {
	if (spawnedHitbox->IsValidLowLevel()) {
		spawnedHitbox->UnregisterComponent();
		spawnedHitbox->DestroyComponent();
		spawnedHitbox = nullptr;
	}
	Received_NotifyEnd(MeshComponent, AnimationSequence);
}

void UAnimNotifyState_SpawnHitbox::Tick(float DeltaTime) {
	if (currentMeshComponent->IsValidLowLevel() && currentAnimation->IsValidLowLevel()) {
		float currentTime = currentMeshComponent->GetAnimInstance()->Montage_GetPosition(currentAnimation);

		if (currentTime >= notifyStartTime && currentTime <= notifyEndTime) {
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