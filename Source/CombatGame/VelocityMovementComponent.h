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
		float groundDistance = 1.0f;
	UPROPERTY(EditAnywhere)
		float friction = 6.0f;
	UPROPERTY(EditAnywhere)
		float acceleration = 3000.0f;
	UPROPERTY(EditAnywhere)
		float gravitySpeed = 500.0f;
	UPROPERTY(EditAnywhere)
		float maxGravity = 10000.0f;
	UPROPERTY(EditAnywhere)
		float jumpHeight = 100.0f;
	UPROPERTY(EditAnywhere)
		float jumpSpeed = 6.0f;
	UPROPERTY(EditAnywhere)
		float knockbackSpeed = 12.0f;
	UPROPERTY(EditAnywhere)
		float extraKnockbackAirtime = 200.0f;

	bool bInHitstun;
	float currentHorizontalKnockback;

	bool bGroundedLastFrame;
	bool bTickOffWalkingNextFrame;
	bool bIsWalking;

	bool bIsJumping;

	FVector velocity;
	float halfHeight;

	UFUNCTION()
		void EnterGround();

public:
	UPROPERTY(BlueprintAssignable)
		FEnterGroundDelegate OnEnterGround;

	UPROPERTY(BlueprintAssignable)
		FLeaveGroundDelegate OnLeaveGround;

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
		void Jump();

	UFUNCTION(BlueprintCallable)
		void ApplyKnockback(FVector knockbackVelocity);

	UFUNCTION(BlueprintCallable)
		FVector GetOwnerLocation();
};
