// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerPawn.h"

void APlayerPawn::EnableCombo() {
	canCombo = true;
	if (starterAttackCount < maxStarterAttacks) {
		starterAttackCount += 1;
	}
	else if(finisherAttackCount < maxFinisherAttacks){
		finisherAttackCount += 1;
	}
}

void APlayerPawn::ResetCombo() {
	starterAttackCount = 0;
	finisherAttackCount = 0;
	canCombo = false;
}

void APlayerPawn::GroundAttack() {
	if (!IsAttacking()) {
		if (starterAttackCount < maxStarterAttacks) {
			DoAttackMontage(groundAttacks.starters[0], true);
		}
		else if (finisherAttackCount < maxFinisherAttacks) {
			DoAttackMontage(groundAttacks.finishers[0], true);
		}
	}
	else if (canCombo) {
		canCombo = false;
		if (starterAttackCount < maxStarterAttacks) {
			DoAttackMontage(groundAttacks.starters[0], true);
		}
		else if(finisherAttackCount < maxFinisherAttacks) {
			DoAttackMontage(groundAttacks.finishers[0], true);
		}
	}
}