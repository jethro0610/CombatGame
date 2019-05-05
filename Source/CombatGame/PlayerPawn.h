// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombatPawn.h"
#include "PlayerPawn.generated.h"

USTRUCT()
struct FAttackGroup {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
		FName attackName;

	UPROPERTY(EditAnywhere)
		TArray<UAnimMontage*> starters;

	UPROPERTY(EditAnywhere)
		TArray<UAnimMontage*> finishers;
};

UCLASS()
class COMBATGAME_API APlayerPawn : public ACombatPawn
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
		int maxStarterAttacks = 2;

	UPROPERTY(EditAnywhere)
		int maxFinisherAttacks = 1;

	UPROPERTY(EditAnywhere)
		FAttackGroup groundAttacks;

	int starterAttackCount;
	int finisherAttackCount;
	bool canCombo;

public:
	UFUNCTION(BlueprintCallable)
		void GroundAttack();

	UFUNCTION(BlueprintCallable)
		void EnableCombo();

	UFUNCTION(BlueprintCallable)
		void ResetCombo();
};
