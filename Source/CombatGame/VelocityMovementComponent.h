#pragma once

#include "CoreMinimal.h"
#include "GameFramework/MovementComponent.h"
#include "VelocityMovementComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEnterGroundDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLeaveGroundDelegate);
UCLASS(BlueprintType, meta = (BlueprintSpawnableComponent), DisplayName = "Velocity Movement Component")
class COMBATGAME_API UVelocityMovementComponent : public UMovementComponent
{
	GENERATED_BODY()
	
public:
	UVelocityMovementComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
		float groundDistance = 5.0f;
	UPROPERTY(EditAnywhere)
		float maxGroundSpeed = 7.0f;
	UPROPERTY(EditAnywhere)
		float friction = 0.15f;
	UPROPERTY(EditAnywhere)
		bool bMoveInAir;
	UPROPERTY(EditAnywhere)
		float maxAirSpeed = 7.0f;
	UPROPERTY(EditAnywhere)
		float aerialFriction = 0.025f;
	UPROPERTY(EditAnywhere)
		float gravitySpeed = 0.07f;
	UPROPERTY(EditAnywhere)
		float maxGravity = 100.0f;
	UPROPERTY(EditAnywhere)
		float jumpStrength = 15.0f;
	UPROPERTY(EditAnywhere)
		float jumpResistance = 3.0f;
	UPROPERTY(EditAnywhere)
		float knockbackStrength = 1.0f;
	UPROPERTY(EditAnywhere)
		float knockbackResistance = 20.0f;
	UPROPERTY(EditAnywhere)
		float extraKnockbackAirtime = 5.0f;
	UPROPERTY(EditAnywhere)
		float speedInHitlag = 0.075f;

	float acceleration;
	float aerialAcceleration;
	bool bShouldMoveInAir;

	float substepTime = 0.0f;
	float substepTickRate = 144.0f;
	float substepBank = 0.0f;
	FVector previousPosition;
	FVector currentPosition;
	FVector interpolatedPosition;

	float currentHitlag;
	bool bInHitstun;

	float currentHorizontalKnockback;
	float currentVerticalKnockback;

	bool bGroundedLastFrame;
	bool bIsJumping;

	FVector walkVector;
	FVector velocity;
	float halfHeight;

	void CalculateMovement();

	UFUNCTION()
		void EnterGround();

public:
	UPROPERTY(BlueprintAssignable)
		FEnterGroundDelegate OnEnterGround;

	UPROPERTY(BlueprintAssignable)
		FLeaveGroundDelegate OnLeaveGround;

	UPROPERTY(EditAnywhere)
		bool bGravityEnabled = true;

	UFUNCTION(BlueprintCallable)
		void SetHalfHeight(float newHalfHeight);
	UFUNCTION(BlueprintCallable)
		FHitResult GetGroundTrace();
	UFUNCTION(BlueprintCallable)
		bool IsOnGround();
	UFUNCTION(BlueprintCallable)
		bool IsWalking();
	UFUNCTION(BlueprintCallable)
		bool IsJumping();

	UFUNCTION(BlueprintCallable)
		FVector GetVelocity();
	UFUNCTION(BlueprintCallable)
		FVector GetVelocityNoGravity();
	UFUNCTION(BlueprintCallable)
		void AddVelocity(FVector deltaVelocity);
	UFUNCTION(BlueprintCallable)
		void SetVelocity(FVector newVelocity);
	UFUNCTION(BlueprintCallable)
		void SetVelocityNoGravity(FVector newVelocity);
	UFUNCTION(BlueprintCallable)
		void SetGravityEnabled(bool gravityEnabled);
	UFUNCTION(BlueprintCallable)
		float GetGravity();
	UFUNCTION(BlueprintCallable)
		void AddGravity(float deltaGravity);
	UFUNCTION(BlueprintCallable)
		void SetGravity(float newGravity);
	UFUNCTION(BlueprintCallable)
		bool CanMoveInAir();
	UFUNCTION(BlueprintCallable)
		void SetMoveInAir(bool moveInAir);
	UFUNCTION(BlueprintCallable)
		void ApplyHitlag(float secondsOfHitlag);
	UFUNCTION(BlueprintCallable)
		bool IsInHitlag();

	UFUNCTION(BlueprintCallable)
		FVector GetInterpolatedPosition();

	UFUNCTION(BlueprintCallable)
		void Walk(FVector walkDirection, float walkSpeed);

	UFUNCTION(BlueprintCallable)
		void Move(FVector deltaVector);

	UFUNCTION(BlueprintCallable)
		void Jump();

	UFUNCTION(BlueprintCallable)
		void ApplyKnockback(FVector knockbackVelocity);

	UFUNCTION(BlueprintCallable)
		FVector GetOwnerLocation();
};
