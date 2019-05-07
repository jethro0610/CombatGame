#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "VelocityMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "CombatComponent.h"
#include "Animation/AnimMontage.h"
#include "CombatPawn.generated.h"

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

public:
	UPROPERTY(BlueprintAssignable)
		FAttackDelegate OnAttack;

protected:
	UPROPERTY(VisibleAnywhere)
		UCapsuleComponent* collisionCapsule;

	UPROPERTY(VisibleAnywhere)
		USkeletalMeshComponent* skeletalMesh;

	UPROPERTY(VisibleAnywhere)
		UCombatComponent* combatComponent;

	UPROPERTY(VisibleAnywhere)
		UVelocityMovementComponent* movementComponent;

	UPROPERTY(EditAnywhere)
		bool bApplyRootMotion = true;

	UPROPERTY(EditAnywhere)
		bool bRecieveKnockback = true;

private:
	UFUNCTION()
		void HitByAttack(UHurtbox* hitCollider, UHitbox* attackingCollider, FHitResult hitResult);

	TWeakObjectPtr<UAnimMontage> currentAttackMontage;

public:
	UFUNCTION(BlueprintCallable)
		UVelocityMovementComponent* GetMovement();

	UFUNCTION(BlueprintCallable)
		UCombatComponent* GetCombatComponent();

	UFUNCTION(BlueprintCallable)
		UAnimMontage* GetCurrentMontage();

	UFUNCTION(BlueprintCallable)
		bool IsAttacking();

	UFUNCTION(BlueprintCallable)
		void DoAttackMontage(UAnimMontage* attackMontage, bool interruptCurrentAttack);

	UFUNCTION(BlueprintCallable)
		void CancelAttack();
};
