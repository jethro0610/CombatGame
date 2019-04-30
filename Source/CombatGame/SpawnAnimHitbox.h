// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Stats.h"
#include "UObject/ObjectMacros.h"
#include "UObject/Object.h"
#include "Tickable.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "CombatCollider.h"
#include "Components/CapsuleComponent.h"
#include "SpawnAnimHitbox.generated.h"

/**
 * 
 */
UCLASS()
class COMBATGAME_API USpawnAnimHitbox : public UAnimNotifyState, public FTickableGameObject
{
	GENERATED_BODY()
	
public:
	USpawnAnimHitbox();
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComponent, UAnimSequenceBase* AnimationSequence, float TotalDuration);
	virtual void NotifyTick(USkeletalMeshComponent* MeshComponent, UAnimSequenceBase* AnimationSequence, float FrameDeltaTime);
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComponent, UAnimSequenceBase* AnimationSequence);

public:
	virtual void Tick(float DeltaTime) override;
	virtual bool IsTickable() const override;
	virtual bool IsTickableInEditor() const override;
	virtual bool IsTickableWhenPaused() const override;
	virtual TStatId GetStatId() const override;

public:
	UPROPERTY(EditAnywhere)
		FName socket;

	UPROPERTY(EditAnywhere)
		FVector offset;

	UPROPERTY(EditAnywhere)
		FRotator rotation;

	UPROPERTY(EditAnywhere)
		float length;

	UPROPERTY(EditAnywhere)
		float width;

	UPROPERTY(EditAnywhere)
		ECombatColliderType type;

	UPROPERTY(EditAnywhere)
		bool isIntangible;

	UPROPERTY(EditAnywhere)
		FName attackGroup;

	UPROPERTY(EditAnywhere)
		float damage;

	UPROPERTY(EditAnywhere)
		float horizontalKnockback;

	UPROPERTY(EditAnywhere)
		float verticalKnockback;

private:
	USkeletalMeshComponent* currentMeshComponent;
	UAnimMontage* currentAnimation;
	float notifyStartTime;
	float notifyEndTime;
	UCombatCollider* spawnedCollider;
};
