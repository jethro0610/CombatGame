#pragma once

#include "CoreMinimal.h"
#include "GameFramework/MovementComponent.h"
#include "VelocityMovementComponent.generated.h"


UCLASS()
class COMBATGAME_API UVelocityMovementComponent : public UMovementComponent
{
	GENERATED_BODY()
	
public:
	UVelocityMovementComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(EditAnywhere)
		float groundDistance = 1.0f;
	UPROPERTY(EditAnywhere)
		float friction = 6.0f;
	UPROPERTY(EditAnywhere)
		float acceleration = 3000.0f;

	FVector velocity;

public:
	UPROPERTY(EditAnywhere)
		bool bCanWalkInAir = false;
	UPROPERTY(EditAnywhere)
		bool bFrictionInAir = false;

	UFUNCTION(BlueprintCallable)
		FHitResult GetGroundTrace();
	UFUNCTION(BlueprintCallable)
		bool IsOnGround();

	UFUNCTION(BlueprintCallable)
		FVector GetVelocity();
	UFUNCTION(BlueprintCallable)
		FVector GetVelocityNoGravity();
	UFUNCTION(BlueprintCallable)
		void AddVelocity(FVector deltaVelocity);
	UFUNCTION(BlueprintCallable)
		void SetVelocity(FVector newVelocity);

	UFUNCTION(BlueprintCallable)
		void Walk(FVector walkDirection);

	UFUNCTION(BlueprintCallable)
		void Move(FVector deltaVector);

	UFUNCTION(BlueprintCallable)
		FVector GetOwnerLocation();
};
