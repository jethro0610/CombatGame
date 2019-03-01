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

UVelocityMovementComponent* AMovingPawn::GetMovement() {
	return movementComponent;
}
