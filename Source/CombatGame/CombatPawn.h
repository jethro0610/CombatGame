#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "VelocityMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "CombatComponent.h"
#include "Animation/AnimMontage.h"
#include "CombatPawn.generated.h"

USTRUCT()
struct FAttack {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
		FName attackName;

	UPROPERTY(EditAnywhere)
		TArray<UAnimMontage*> attackComboOrder;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAttackDelegate);

UCLASS()
class COMBATGAME_API ACombatPawn : public APawn
{
	GENERATED_BODY()

public:
	ACombatPawn();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(BlueprintAssignable)
		FAttackDelegate OnAttack;

private:
	UPROPERTY(VisibleAnywhere)
		UCapsuleComponent* collisionCapsule;

	UPROPERTY(VisibleAnywhere)
		UCombatComponent* combatComponent;

	UPROPERTY(VisibleAnywhere)
		UVelocityMovementComponent* movementComponent;

	UPROPERTY(EditAnywhere)
		TArray<FAttack> attacks;

	int currentComboLength;
	bool canCombo;

public:
	UFUNCTION(BlueprintCallable)
		UVelocityMovementComponent* GetMovement();

	UFUNCTION(BlueprintCallable)
		UCombatComponent* GetCombatComponent();

	UFUNCTION(BlueprintCallable)
		void PlayAnimMontage(UAnimMontage* animMontage);

	UFUNCTION(BlueprintCallable)
		UAnimMontage* GetCurrentMontage();

	UFUNCTION(BlueprintCallable)
		bool MontageIsAttack(UAnimMontage* inMontage);

	UFUNCTION(BlueprintCallable)
		bool IsAttacking();

	UFUNCTION(BlueprintCallable)
		int GetAttackIndexFromName(FName atackName);

	UFUNCTION(BlueprintCallable)
		void DoAttack(FName attackName);

	UFUNCTION(BlueprintCallable)
		void EnableCombo();

	UFUNCTION(BlueprintCallable)
		void ResetCombo();
};
