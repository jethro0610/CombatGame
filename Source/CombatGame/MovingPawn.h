#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "VelocityMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "MovingPawn.generated.h"

UCLASS()
class COMBATGAME_API AMovingPawn : public APawn
{
	GENERATED_BODY()

public:
	AMovingPawn();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UPROPERTY(EditAnywhere)
		UCapsuleComponent* collisionCapsule;

	UPROPERTY(EditAnywhere)
		UVelocityMovementComponent* movementComponent;

public:
	FHitResult GetGroundTrace();
	bool IsOnGround();

	FVector GetVelocity();
	FVector GetVelocityNoGravity();
	void AddVelocity(FVector deltaVelocity);
	void SetVelocity(FVector newVelocity);

	void Walk(FVector walkDirection, float DeltaTime);
	void Move(FVector deltaVector);
};
