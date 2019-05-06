// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombatPawn.h"
#include "PlayerPawn.generated.h"

USTRUCT(BlueprintType)
struct FAttackGroup {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
		FName attackGroupName;

	UPROPERTY(EditAnywhere)
		TArray<UAnimMontage*> starters;

	UPROPERTY(EditAnywhere)
		TArray<UAnimMontage*> finishers;
};

UCLASS()
class COMBATGAME_API APlayerPawn : public ACombatPawn
{
	GENERATED_BODY()
public:
	APlayerPawn();

private:
	UPROPERTY(EditAnywhere)
		int maxStarterAttacks = 2;

	UPROPERTY(EditAnywhere)
		int maxFinisherAttacks = 1;

	UPROPERTY(EditAnywhere)
		TArray<FAttackGroup> attackGroups;

	int starterAttackCount;
	int finisherAttackCount;
	bool bCanCombo;

public:
	UFUNCTION(BlueprintCallable)
		FAttackGroup GetAttackGroupFromName(FName attackGroupName);

	UFUNCTION(BlueprintCallable)
		void DoAttackFromGroup(FName attackGroupName);

	UFUNCTION(BlueprintCallable)
		void EnableCombo();

	UFUNCTION(BlueprintCallable)
		void ResetCombo();
};
