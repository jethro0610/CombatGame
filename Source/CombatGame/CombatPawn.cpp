#include "CombatPawn.h"
#include "Animation/AnimInstance.h"

ACombatPawn::ACombatPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	collisionCapsule = CreateDefaultSubobject<UCapsuleComponent>("World Collider");
	collisionCapsule->SetCollisionProfileName("BlockAllDynamic");
	RootComponent = collisionCapsule;

	skeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>("Mesh");
	skeletalMesh->AttachTo(RootComponent);

	combatComponent = CreateDefaultSubobject<UCombatComponent>("Combat Component");

	movementComponent = CreateDefaultSubobject<UVelocityMovementComponent>("Movement Component");
	movementComponent->SetHalfHeight(collisionCapsule->GetScaledCapsuleHalfHeight());
	movementComponent->SetUpdatedComponent(RootComponent);
}

void ACombatPawn::BeginPlay()
{
	Super::BeginPlay();
	combatComponent->OnHitByAttack.AddDynamic(this, &ACombatPawn::HitByAttack);
}

void ACombatPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bApplyRootMotion && movementComponent != nullptr && skeletalMesh != nullptr) {
		FTransform rootTransform = skeletalMesh->ConsumeRootMotion().GetRootMotionTransform();
		movementComponent->Move(skeletalMesh->GetComponentRotation().RotateVector(rootTransform.GetLocation()));
	}

	if (currentAttackMontage != nullptr && GetCurrentMontage() == nullptr)
		currentAttackMontage = nullptr;
}

void ACombatPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ACombatPawn::HitByAttack(UHurtbox* hitCollider, UHitbox* attackingCollider, FHitResult hitResult) {
	if (bRecieveKnockback)
		movementComponent->ApplyKnockback(combatComponent->GetKnockbackVector(hitCollider, attackingCollider));
}

UCombatComponent* ACombatPawn::GetCombatComponent() {
	return combatComponent;
}

UVelocityMovementComponent* ACombatPawn::GetMovement() {
	return movementComponent;
}

UAnimMontage* ACombatPawn::GetCurrentMontage() {
	return skeletalMesh->GetAnimInstance()->GetCurrentActiveMontage();
}

bool ACombatPawn::IsAttacking() {
	if (GetCurrentMontage() != nullptr) {
		if (GetCurrentMontage() == currentAttackMontage) {
			return true;
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
}

void ACombatPawn::DoAttackMontage(UAnimMontage* attackMontage, bool interruptCurrentAttack) {
	if (!IsAttacking() || interruptCurrentAttack) {
		skeletalMesh->GetAnimInstance()->Montage_Play(attackMontage);
		currentAttackMontage = attackMontage;
		OnAttack.Broadcast();
	}
}