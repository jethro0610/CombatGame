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
}

void UVelocityMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	substepBank += (DeltaTime) / (1.0f / substepTickRate);
	int numberOfTicks = FMath::RoundToInt(substepBank);
	substepBank -= numberOfTicks;
	for (int i = 0; i < numberOfTicks; i++) {
		CalculateMovement();
	}
	walkVector = FVector::ZeroVector;
}

void UVelocityMovementComponent::CalculateMovement() {
	if (currentHitlag <= 0.0f) {
		currentHitlag = 0.0f;
		if (IsOnGround()) {
			if (!bInHitstun) {
				AddVelocity(walkVector);
				SetVelocityNoGravity(GetVelocityNoGravity() * (1.0f - friction));
			}
			else {
				SetVelocityNoGravity(GetVelocityNoGravity() / (1.0f + (knockbackResistance / 100.0f)));
				if (GetVelocityNoGravity().Size() <= 0.1f) {
					bInHitstun = false;
				}
			}
			if (GetGravity() > 0.0f)
				SetGravity(0.0f);

			if (!bGroundedLastFrame) {
				bGroundedLastFrame = true;
				OnEnterGround.Broadcast();
			}
		}
		else {
			if (bFrictionInAir)
				SetVelocityNoGravity(GetVelocityNoGravity() * (1.0f - friction));

			if (bGravityEnabled)
				AddGravity(gravitySpeed);

			if (bInHitstun) {
				bool verticalStunOver = true;
				bool horizontalStunOver = true;
				if (GetGravity() < -currentVerticalKnockback / (knockbackResistance / 1.5f)) {
					SetGravity(GetGravity() / (1.0f + (knockbackResistance / 100.0f)));
					verticalStunOver = false;
				}

				if (GetVelocityNoGravity().Size() > currentHorizontalKnockback / (knockbackResistance / 4.0f)) {
					SetVelocityNoGravity(GetVelocityNoGravity() / (1.0f + (knockbackResistance / 100.0f)));
					horizontalStunOver = false;
				}

				if (verticalStunOver && horizontalStunOver && GetGravity() > 0.0f)
					bInHitstun = false;
			}

			if (bIsJumping) {
				if (GetGravity() < 0.0f) {
					SetGravity(GetGravity() / (1.0f + (jumpResistance / 100.0f)));
				}
				else {
					bIsJumping = false;
				}
			}

			if (bGroundedLastFrame) {
				bGroundedLastFrame = false;
				OnLeaveGround.Broadcast();
			}
		}
		Move(velocity);
	}
	else {
		currentHitlag -= 1.0f / substepTickRate;
	}
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
	return (walkVector.Size() > 0.05f);
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

void UVelocityMovementComponent::ApplyHitlag(int framesOfHitlag) {
	currentHitlag = (1.0f/substepTickRate) * framesOfHitlag;
}

bool UVelocityMovementComponent::IsInHitlag() {
	return (currentHitlag > 0.0f);
}

void UVelocityMovementComponent::Walk(FVector walkDirection, float walkSpeed) {
	float DeltaSeconds = GetWorld()->DeltaTimeSeconds * 60.0f;
	FVector oneWalkDirection = walkDirection.GetClampedToMaxSize(1.0f);
	if (IsOnGround() || bCanWalkInAir) {
		walkVector = oneWalkDirection * walkSpeed * acceleration;
		if (walkVector.Size() < 0.1f)
			walkVector == FVector::ZeroVector;
	}
	else {
		walkVector = FVector::ZeroVector;
	}
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