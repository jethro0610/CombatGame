#include "MovingPawn.h"

AMovingPawn::AMovingPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	collisionCapsule = CreateDefaultSubobject<UCapsuleComponent>("Collider");
	RootComponent = collisionCapsule;

	movementComponent = CreateDefaultSubobject<UVelocityMovementComponent>("Movement Component");
	movementComponent->SetUpdatedComponent(RootComponent);
}

void AMovingPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMovingPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMovingPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

FHitResult AMovingPawn::GetGroundTrace() {
	return movementComponent->GetGroundTrace();
}

bool AMovingPawn::IsOnGround() {
	return movementComponent->IsOnGround();
}

FVector AMovingPawn::GetVelocity() {
	return movementComponent->GetVelocity();
}

FVector AMovingPawn::GetVelocityNoGravity() {
	return movementComponent->GetVelocityNoGravity();
}

void AMovingPawn::AddVelocity(FVector deltaVelocity) {
	movementComponent->AddVelocity(deltaVelocity);
}

void AMovingPawn::SetVelocity(FVector newVelocity) {
	movementComponent->SetVelocity(newVelocity);
}

void AMovingPawn::Walk(FVector walkDirection, float DeltaTime) {
	movementComponent->Walk(walkDirection, DeltaTime);
}

void AMovingPawn::Move(FVector deltaVector) {
	movementComponent->Move(deltaVector);
}
