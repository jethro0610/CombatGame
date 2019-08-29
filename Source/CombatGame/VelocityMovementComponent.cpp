#include "VelocityMovementComponent.h"
#include "Engine.h"
#include "Kismet/KismetMathLibrary.h"

UVelocityMovementComponent::UVelocityMovementComponent() {
	PrimaryComponentTick.bCanEverTick = true;
}

void UVelocityMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	OnEnterGround.AddDynamic(this, &UVelocityMovementComponent::EnterGround);

	previousPosition = GetOwner()->GetActorLocation();
	currentPosition = GetOwner()->GetActorLocation();
	interpolatedPosition = GetOwner()->GetActorLocation();

	bShouldMoveInAir = bMoveInAir;

	acceleration = maxGroundSpeed * friction;
	aerialAcceleration = maxAirSpeed * aerialFriction;
}

void UVelocityMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Set speed in hitlag
	float speed = 1.0f;
	if (currentHitlag <= 0.0f) {
		currentHitlag = 0.0f;
	}
	else {
		currentHitlag -= DeltaTime;
		speed = speedInHitlag;
	}

	// Add to substep bank by delta time (time between frames)
	substepBank += ((DeltaTime) / (1.0f / substepTickRate)) * speed;
	// Tick if bank is greater than 1
	int numberOfTicks = FMath::RoundFromZero(substepBank);
	if (substepBank >= 1.0f) {
		substepBank -= numberOfTicks;
	}
	else {
		numberOfTicks = 0;
	}
	for (int i = 0; i < numberOfTicks; i++) {
		CalculateMovement();
	}

	// Interpolate to midpoint of true position and last one
	desiredMovement = FVector::ZeroVector;
	interpolatedPosition = FMath::Lerp(previousPosition, currentPosition, substepBank);
}

void UVelocityMovementComponent::CalculateMovement() {
	substepTime = GetWorld()->TimeSeconds;
	if (IsOnGround()) {
		if (!bInHitstun) {
			// Move and apply friction
			AddVelocity(desiredMovement * acceleration);
			SetVelocityNoGravity(GetVelocityNoGravity() * (1.0f - friction));
		}
		else {
			// Decrease knockback
			SetVelocityNoGravity(GetVelocityNoGravity() / (1.0f + (knockbackResistance / 100.0f)));
			if (GetVelocityNoGravity().Size() <= 0.1f) {
				// End stun on landing
				bInHitstun = false;
			}
		}
		// Clamp gravity
		if (GetGravity() > 0.0f)
			SetGravity(0.0f);

		// Ground delegate detection
		if (!bGroundedLastFrame) {
			bGroundedLastFrame = true;
			OnEnterGround.Broadcast();
		}
	}
	else {
		if (!bInHitstun && CanMoveInAir()) {
			// Move and apply friction
			AddVelocity(desiredMovement * aerialAcceleration);
			SetVelocityNoGravity(GetVelocityNoGravity() * (1.0f - aerialFriction));
		}

		// Apply gravity
		if (bGravityEnabled)
			AddGravity(gravitySpeed);

		// Decrease knockback
		if (bInHitstun) {
			if (GetGravity() < -currentVerticalKnockback / (knockbackResistance / 1.5f)) {
				SetGravity(GetGravity() / (1.0f + (knockbackResistance / 100.0f)));
			}

			if (GetVelocityNoGravity().Size() > currentHorizontalKnockback / (knockbackResistance / 4.0f)) {
				SetVelocityNoGravity(GetVelocityNoGravity() / (1.0f + (knockbackResistance / 100.0f)));
			}
		}

		// Jump physics
		if (bIsJumping) {
			if (GetGravity() < 0.0f) {
				SetGravity(GetGravity() / (1.0f + (jumpResistance / 100.0f)));
			}
			else {
				bIsJumping = false;
			}
		}

		// Ground delegate detection
		if (bGroundedLastFrame) {
			bGroundedLastFrame = false;
			OnLeaveGround.Broadcast();
		}
	}
	// Apply velocity
	Move(velocity);

	// Interpolate rotation
	FRotator currentDesiredRotation = desiredRotation;
	FRotator lerpRotation = FQuat::Slerp(GetOwner()->GetActorRotation().Quaternion(), desiredRotation.Quaternion(), 1.0f - FMath::Exp(-rotationSpeed)).Rotator();
	GetOwner()->SetActorRotation(lerpRotation);
	desiredRotation = currentDesiredRotation;

	previousPosition = currentPosition;
	currentPosition = GetOwner()->GetActorLocation();
}

void UVelocityMovementComponent::EnterGround() {
	if (bInHitstun)
		SetVelocity(FVector::ZeroVector);
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

bool UVelocityMovementComponent::IsWalking() {
	return IsOnGround() && desiredMovement.Size() > 0.05f;
}

bool UVelocityMovementComponent::IsJumping() {
	return bIsJumping;
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

void UVelocityMovementComponent::SetVelocityNoGravity(FVector newVelocity) {
	FVector newVelocityNoGravity = FVector(newVelocity.X, newVelocity.Y, GetVelocity().Z);
	velocity = newVelocityNoGravity;
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
	float updatedGravity = -newGravity;
	updatedGravity = FMath::Max(-maxGravity, updatedGravity);
	velocity = FVector(velocity.X, velocity.Y, updatedGravity);
}

bool UVelocityMovementComponent::CanMoveInAir() {
	return bMoveInAir && bShouldMoveInAir;
}

void UVelocityMovementComponent::SetMoveInAir(bool moveInAir) {
	bShouldMoveInAir = moveInAir;
}

void UVelocityMovementComponent::ApplyHitlag(float secondsOfHitlag) {
	currentHitlag = secondsOfHitlag;
}

bool UVelocityMovementComponent::IsInHitlag() {
	return (currentHitlag > 0.0f);
}

FVector UVelocityMovementComponent::GetInterpolatedPosition() {
	return interpolatedPosition;
}

void UVelocityMovementComponent::SetDesiredMovement(FVector movementDirection, float movementSpeed) {
	FVector oneMovementDirection = movementDirection.GetClampedToMaxSize(1.0f);
	desiredMovement = oneMovementDirection * movementSpeed;
	if (desiredMovement.Size() < 0.1f)
		desiredMovement == FVector::ZeroVector;
}

void UVelocityMovementComponent::SetDesiredRotation(FRotator newRotation) {
	desiredRotation = newRotation;
}

void UVelocityMovementComponent::Move(FVector deltaVector) {
	FHitResult movementHit;
	SafeMoveUpdatedComponent(deltaVector, UpdatedComponent->GetComponentRotation(), true, movementHit);
	if (movementHit.IsValidBlockingHit()) {
		SlideAlongSurface(deltaVector, 1.0f - movementHit.Time, movementHit.Normal, movementHit, false);
	}
}

void UVelocityMovementComponent::Jump() {
	if (IsOnGround()) {
		SetGravity(-jumpStrength);
		bIsJumping = true;
	}
}

void UVelocityMovementComponent::ApplyKnockback(FVector knockbackVelocity) {
	SetVelocity(knockbackVelocity * knockbackStrength);
	currentHorizontalKnockback = FVector(knockbackVelocity.X, knockbackVelocity.Y, 0.0f).Size() * knockbackStrength;
	currentVerticalKnockback = knockbackVelocity.Z * knockbackStrength;
	bInHitstun = true;
}

FVector UVelocityMovementComponent::GetOwnerLocation() {
	return GetOwner()->GetActorLocation();
}