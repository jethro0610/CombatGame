// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerPawn.h"

APlayerPawn::APlayerPawn() {
	FAttackGroup groundAttacks;
	groundAttacks.attackGroupName = FName("Ground Attacks");
	attackGroups.Add(groundAttacks);

	FAttackGroup airAttacks;
	airAttacks.attackGroupName = FName("Air Attacks");
	attackGroups.Add(airAttacks);
}

void APlayerPawn::EnableCombo() {
	if (starterAttackCount < maxStarterAttacks) {
		starterAttackCount += 1;
	}
	else if(finisherAttackCount < maxFinisherAttacks){
		finisherAttackCount += 1;
	}
	bCanCombo = true;
}

void APlayerPawn::ResetCombo() {
	starterAttackCount = 0;
	finisherAttackCount = 0;
	bCanCombo = false;
}

FAttackGroup APlayerPawn::GetAttackGroupFromName(FName attackGroupName) {
	FAttackGroup returnGroup;
	for (int i = 0; i < attackGroups.Num(); i++)
	{
		if (attackGroups[i].attackGroupName == attackGroupName) {
			returnGroup = attackGroups[i];
		}
	}
	return returnGroup;
}

void APlayerPawn::DoAttackFromGroup(FName attackGroupName) {
	FAttackGroup currentAttackGroup = GetAttackGroupFromName(attackGroupName);
	if (!IsAttacking() || bCanCombo) {
		if (starterAttackCount < maxStarterAttacks && currentAttackGroup.starters.Num() > 0) {
			int attackIndex = starterAttackCount % currentAttackGroup.starters.Num();
			DoAttackMontage(currentAttackGroup.starters[attackIndex], true);
		}
		else if (finisherAttackCount < maxFinisherAttacks && currentAttackGroup.finishers.Num() > 0) {
			int attackIndex = finisherAttackCount % currentAttackGroup.finishers.Num();
			DoAttackMontage(currentAttackGroup.finishers[attackIndex], true);
		}
		bCanCombo = false;
	}
}