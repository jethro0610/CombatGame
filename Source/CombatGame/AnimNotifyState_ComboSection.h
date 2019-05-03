// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "CombatPawn.h"
#include "AnimNotifyState_ComboSection.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName = "Combo Section"))
class COMBATGAME_API UAnimNotifyState_ComboSection : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComponent, UAnimSequenceBase* AnimationSequence, float TotalDuration);
	virtual void NotifyTick(USkeletalMeshComponent* MeshComponent, UAnimSequenceBase* AnimationSequence, float FrameDeltaTime);
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComponent, UAnimSequenceBase* AnimationSequence);

private:
	bool isInitialTick;
	bool hasAttacked;
	ACombatPawn* owningPawn;

private:
	UFUNCTION()
		void OnPawnAttack();
};
