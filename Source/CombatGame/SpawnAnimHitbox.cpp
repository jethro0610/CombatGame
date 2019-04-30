// Fill out your copyright notice in the Description page of Project Settings.

#include "SpawnAnimHitbox.h"
#include "Components/SkeletalMeshComponent.h"
#include "Classes/Animation/AnimInstance.h"
#include "Engine.h"
#include "CombatPawn.h"
#include "DrawDebugHelpers.h"

USpawnAnimHitbox::USpawnAnimHitbox() {

}

void USpawnAnimHitbox::NotifyBegin(USkeletalMeshComponent* MeshComponent, UAnimSequenceBase* AnimationSequence, float TotalDuration) {
	ACombatPawn* owningPawn = Cast<ACombatPawn,AActor>(MeshComponent->GetOwner());
	if(owningPawn->IsValidLowLevel())
		spawnedCollider = owningPawn->GetCombatComponent()->SpawnCombatCollider(MeshComponent, socket, offset, length, width, type, isIntangible, attackGroup, damage, horizontalKnockback, verticalKnockback);

	Received_NotifyBegin(MeshComponent, AnimationSequence, TotalDuration);
}

void USpawnAnimHitbox::NotifyTick(USkeletalMeshComponent* MeshComponent, UAnimSequenceBase* AnimationSequence, float FrameDeltaTime) {
	currentMeshComponent = MeshComponent;
	currentAnimation = Cast<UAnimMontage, UAnimSequenceBase>(AnimationSequence);

	notifyStartTime = currentMeshComponent->GetAnimInstance()->ActiveAnimNotifyState[0].GetTriggerTime();
	notifyEndTime = currentMeshComponent->GetAnimInstance()->ActiveAnimNotifyState[0].GetEndTriggerTime();

	Received_NotifyTick(MeshComponent, AnimationSequence, FrameDeltaTime);
}

void USpawnAnimHitbox::NotifyEnd(USkeletalMeshComponent* MeshComponent, UAnimSequenceBase* AnimationSequence) {
	if (spawnedCollider->IsValidLowLevel()) {
		spawnedCollider->UnregisterComponent();
		spawnedCollider->DestroyComponent();
		spawnedCollider = nullptr;
	}
	Received_NotifyEnd(MeshComponent, AnimationSequence);
}

void USpawnAnimHitbox::Tick(float DeltaTime) {
	if (currentMeshComponent->IsValidLowLevel() && currentAnimation->IsValidLowLevel()) {
		float currentTime = currentMeshComponent->GetAnimInstance()->Montage_GetPosition(currentAnimation);

		if (currentTime >= notifyStartTime && currentTime <= notifyEndTime) {
			FTransform socketTransform = currentMeshComponent->GetSocketTransform(socket);
			FVector colliderPosition = socketTransform.TransformPosition(offset/socketTransform.GetScale3D());
			DrawDebugCapsule(currentMeshComponent->GetWorld(), colliderPosition, length, width, socketTransform.Rotator().Quaternion(), FColor::Red);
		}
	}
}

bool USpawnAnimHitbox::IsTickable() const {
	return true;
}

TStatId USpawnAnimHitbox::GetStatId() const {
	return UObject::GetStatID();
}

bool USpawnAnimHitbox::IsTickableInEditor() const {
	return true;
}

bool USpawnAnimHitbox::IsTickableWhenPaused() const {
	return true;
}