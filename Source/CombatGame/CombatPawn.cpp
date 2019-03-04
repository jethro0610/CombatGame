#include "CombatPawn.h"

ACombatPawn::ACombatPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	collisionCapsule = CreateDefaultSubobject<UCapsuleComponent>("World Collider");
	collisionCapsule->SetCollisionProfileName("BlockAllDynamic");
	RootComponent = collisionCapsule;

	combatComponent = CreateDefaultSubobject<UCombatComponent>("Combat Component");

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

UCombatComponent* ACombatPawn::GetCombatComponent() {
	return combatComponent;
}

UVelocityMovementComponent* ACombatPawn::GetMovement() {
	return movementComponent;
}
