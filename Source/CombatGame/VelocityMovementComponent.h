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
		float groundDistance;

	UPROPERTY(EditAnywhere, meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
		float friction;

	UPROPERTY(EditAnywhere)
		float acceleration;

	FVector velocity;

public:
	FHitResult GetGroundTrace();
	bool IsOnGround();

	FVector GetVelocity();
	FVector GetVelocityNoGravity();
	void AddVelocity(FVector deltaVelocity);
	void SetVelocity(FVector newVelocity);

	void Walk(FVector walkDirection, float DeltaTime);

	void Move(FVector deltaVector);

	FVector GetOwnerLocation();
};
