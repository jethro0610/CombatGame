#include "VelocityMovementComponent.h"
#include "Engine.h"

UVelocityMovementComponent::UVelocityMovementComponent() {
	PrimaryComponentTick.bCanEverTick = true;
}

void UVelocityMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (IsOnGround()) {
		AddVelocity(-(GetVelocityNoGravity() * friction) * DeltaTime);
	}
	else {
		if (bFrictionInAir)
			AddVelocity(-(GetVelocityNoGravity() * friction) * DeltaTime);
	}
	Move(velocity * DeltaTime);
}

FHitResult UVelocityMovementComponent::GetGroundTrace() {
	FHitResult outputHit;
	GetWorld()->LineTraceSingleByChannel(outputHit, GetOwnerLocation(), GetOwnerLocation() + (GetOwner()->GetActorUpVector() * -groundDistance), ECC_WorldDynamic);
	return outputHit;
}

bool UVelocityMovementComponent::IsOnGround() {
	return GetGroundTrace().IsValidBlockingHit();
}

FVector UVelocityMovementComponent::GetVelocity() {
	return velocity;
}

FVector UVelocityMovementComponent::GetVelocityNoGravity() {
	return FVector(velocity.X, velocity.Y, 0.0f);
}

void UVelocityMovementComponent::AddVelocity(FVector deltaVelocity) {
	velocity += deltaVelocity;
}

void UVelocityMovementComponent::SetVelocity(FVector newVelocity) {
	velocity = newVelocity;
}

void UVelocityMovementComponent::Walk(FVector walkDirection, float DeltaTime) {
	if (IsOnGround() || bCanWalkInAir) {
		AddVelocity(walkDirection * acceleration * DeltaTime);
	}
}

void UVelocityMovementComponent::Move(FVector deltaVector) {
	FHitResult movementHit;
	SafeMoveUpdatedComponent(deltaVector, UpdatedComponent->GetComponentRotation(), true, movementHit);
	if (movementHit.IsValidBlockingHit()) {
		SlideAlongSurface(deltaVector, 1.0f - movementHit.Time, movementHit.Normal, movementHit, false);
	}
}

FVector UVelocityMovementComponent::GetOwnerLocation() {
	return GetOwner()->GetActorLocation();
}