#include "VelocityMovementComponent.h"
#include "Engine.h"

UVelocityMovementComponent::UVelocityMovementComponent() {
	PrimaryComponentTick.bCanEverTick = true;
}

void UVelocityMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	float DeltaSeconds = GetWorld()->DeltaTimeSeconds;
	if (IsOnGround()) {
		AddVelocity(-(GetVelocityNoGravity() * friction) * DeltaSeconds);
		SetGravity(0.0f);
	}
	else {
		if (bFrictionInAir)
			AddVelocity(-(GetVelocityNoGravity() * friction) * DeltaSeconds);

		if(bGravityEnabled)
			AddGravity(gravitySpeed * DeltaSeconds);
	}
	Move(velocity * DeltaSeconds);
}

void UVelocityMovementComponent::SetHalfHeight(float newHalfHeight) {
	halfHeight = newHalfHeight;
}

FHitResult UVelocityMovementComponent::GetGroundTrace() {
	FHitResult outputHit;

	FCollisionQueryParams traceParams;
	traceParams.AddIgnoredActor(GetOwner());

	FVector ownerUpVector = GetOwner()->GetActorUpVector();
	FVector groundTraceEndPoint = GetOwnerLocation() + (ownerUpVector * -halfHeight) + (ownerUpVector * -groundDistance);

	GetWorld()->LineTraceSingleByChannel(outputHit, GetOwnerLocation(), groundTraceEndPoint, ECC_WorldDynamic, traceParams);
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
void UVelocityMovementComponent::SetGravityEnabled(bool gravityEnabled) {
	bGravityEnabled = gravityEnabled;
}

float UVelocityMovementComponent::GetGravity() {
	return -velocity.Z;
}

void UVelocityMovementComponent::AddGravity(float deltaGravity) {
	float updatedGravity = velocity.Z - deltaGravity;
	updatedGravity = FMath::Max(-maxGravity, updatedGravity);
	velocity = FVector(velocity.X, velocity.Y, updatedGravity);
}

void UVelocityMovementComponent::SetGravity(float newGravity) {
	float updatedGravity = newGravity;
	updatedGravity = FMath::Max(-maxGravity, updatedGravity);
	velocity = FVector(velocity.X, velocity.Y, updatedGravity);
}

void UVelocityMovementComponent::Walk(FVector walkDirection) {
	float DeltaSeconds = GetWorld()->DeltaTimeSeconds;
	if (IsOnGround() || bCanWalkInAir) {
		AddVelocity(walkDirection * acceleration * DeltaSeconds);
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