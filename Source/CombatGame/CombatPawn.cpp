#include "CombatPawn.h"

ACombatPawn::ACombatPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	collisionCapsule = CreateDefaultSubobject<UCapsuleComponent>("Collider");
	collisionCapsule->SetCollisionProfileName("BlockAllDynamic");
	RootComponent = collisionCapsule;

	movementComponent = CreateDefaultSubobject<UVelocityMovementComponent>("Movement Component");
	movementComponent->SetHalfHeight(collisionCapsule->GetScaledCapsuleHalfHeight());
	movementComponent->SetUpdatedComponent(RootComponent);
}

void ACombatPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACombatPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACombatPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

UVelocityMovementComponent* ACombatPawn::GetMovement() {
	return movementComponent;
}
