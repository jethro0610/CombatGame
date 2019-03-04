#pragma once

#include "CoreMinimal.h"
#include "GameFramework/MovementComponent.h"
#include "VelocityMovementComponent.generated.h"


UCLASS(BlueprintType, meta = (BlueprintSpawnableComponent), DisplayName = "Velocity Movement Component")
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
	UPROPERTY(EditAnywhere)
		float gravitySpeed = 1500.0f;
	UPROPERTY(EditAnywhere)
		float maxGravity = 10000.0f;

	FVector velocity;
	float halfHeight;

public:
	UPROPERTY(EditAnywhere)
		bool bCanWalkInAir = false;
	UPROPERTY(EditAnywhere)
		bool bFrictionInAir = false;
	UPROPERTY(EditAnywhere)
		bool bGravityEnabled = true;

	UFUNCTION(BlueprintCallable)
		void SetHalfHeight(float newHalfHeight);
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
		void SetGravityEnabled(bool gravityEnabled);
	UFUNCTION(BlueprintCallable)
		float GetGravity();
	UFUNCTION(BlueprintCallable)
		void AddGravity(float deltaGravity);
	UFUNCTION(BlueprintCallable)
		void SetGravity(float newGravity);

	UFUNCTION(BlueprintCallable)
		void Walk(FVector walkDirection, float walkSpeed);

	UFUNCTION(BlueprintCallable)
		void Move(FVector deltaVector);

	UFUNCTION(BlueprintCallable)
		FVector GetOwnerLocation();
};
