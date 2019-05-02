#include "VelocityMovementComponent.h"
#include "Engine.h"
#include "Kismet/KismetMathLibrary.h"

UVelocityMovementComponent::UVelocityMovementComponent() {
	PrimaryComponentTick.bCanEverTick = true;
}

void UVelocityMovementComponent::BeginPlay()
{
	OnEnterGround.AddDynamic(this, &UVelocityMovementComponent::EnterGround);
}

void UVelocityMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	float DeltaSeconds = GetWorld()->DeltaTimeSeconds;
	if (IsOnGround()) {
		if (!bInHitstun) {
			AddVelocity(-(GetVelocityNoGravity() * friction) * DeltaSeconds);
		}
		else {
			AddVelocity(-(GetVelocityNoGravity() * knockbackSpeed) * DeltaSeconds);
			if (GetVelocityNoGravity().Size() <= 0.1f) {
				bInHitstun = false;
			}
		}
		if (GetGravity() > 0.0f)
			SetGravity(0.0f);

		if (bGroundedLastFrame == false) {
			bGroundedLastFrame = true;
			OnEnterGround.Broadcast();
		}
	}
	else {
		if (bFrictionInAir)
			AddVelocity(-(GetVelocityNoGravity() * friction) * DeltaSeconds);

		if(bGravityEnabled)
			AddGravity(gravitySpeed * DeltaSeconds);

		if (bInHitstun) {
			if (GetGravity() < -extraKnockbackAirtime)
				AddGravity(-(GetGravity() * knockbackSpeed) * DeltaSeconds);

			if(GetVelocityNoGravity().Size() > currentHorizontalKnockback)
				AddVelocity(-(GetVelocityNoGravity() * knockbackSpeed) * DeltaSeconds);
		}

		if (bGroundedLastFrame == true) {
			bGroundedLastFrame = false;
		}
	}
	Move(velocity * DeltaSeconds);
	
	if (bTickOffWalkingNextFrame) {
		bTickOffWalkingNextFrame = false;
		bIsWalking = false;
	}

	if (bIsWalking) {
		bTickOffWalkingNextFrame = true;
	}

	if (bApplyRootMotion) {
		UActorComponent* foundComponent = GetOwner()->GetComponentByClass(USkeletalMeshComponent::StaticClass());
		USkeletalMeshComponent* meshComponent = Cast<USkeletalMeshComponent, UActorComponent>(foundComponent);

		if (meshComponent->IsValidLowLevel()) {
			FTransform rootTransform = meshComponent->ConsumeRootMotion().GetRootMotionTransform();
			Move(meshComponent->GetComponentRotation().RotateVector(rootTransform.GetLocation()));
		}
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
	return bIsWalking;
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
	float updatedGravity = -newGravity;
	updatedGravity = FMath::Max(-maxGravity, updatedGravity);
	velocity = FVector(velocity.X, velocity.Y, updatedGravity);
}

void UVelocityMovementComponent::Walk(FVector walkDirection, float walkSpeed) {
	float DeltaSeconds = GetWorld()->DeltaTimeSeconds;
	FVector oneWalkDirection = walkDirection.GetClampedToMaxSize(1.0f);
	if (IsOnGround() || bCanWalkInAir) {
		AddVelocity(oneWalkDirection * walkSpeed * acceleration * DeltaSeconds);
	}
	bIsWalking = true;
}

void UVelocityMovementComponent::Move(FVector deltaVector) {
	FHitResult movementHit;
	SafeMoveUpdatedComponent(deltaVector, UpdatedComponent->GetComponentRotation(), true, movementHit);
	if (movementHit.IsValidBlockingHit()) {
		SlideAlongSurface(deltaVector, 1.0f - movementHit.Time, movementHit.Normal, movementHit, false);
	}
}

void UVelocityMovementComponent::ApplyKnockback(FVector knockbackVelocity) {
	SetVelocity(knockbackVelocity * knockbackSpeed);
	currentHorizontalKnockback = FVector(knockbackVelocity.X, knockbackVelocity.Y, 0.0f).Size();
	bInHitstun = true;
}

FVector UVelocityMovementComponent::GetOwnerLocation() {
	return GetOwner()->GetActorLocation();
}