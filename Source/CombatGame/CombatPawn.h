#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "VelocityMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "CombatComponent.h"
#include "CombatPawn.generated.h"

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

private:
	UPROPERTY(VisibleAnywhere)
		UCapsuleComponent* collisionCapsule;

	UPROPERTY(VisibleAnywhere)
		UCombatComponent* combatComponent;

	UPROPERTY(VisibleAnywhere)
		UVelocityMovementComponent* movementComponent;

public:
	UFUNCTION(BlueprintCallable)
		UVelocityMovementComponent* GetMovement();

	UFUNCTION(BlueprintCallable)
		UCombatComponent* GetCombatComponent();
};
