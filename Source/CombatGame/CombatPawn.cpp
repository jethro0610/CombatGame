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
	movementComponent->SetUpdatedComponent(RootComponent);
}

void ACombatPawn::BeginPlay()
{
	Super::BeginPlay();
	combatComponent->OnHitByAttack.AddDynamic(this, &ACombatPawn::HitByAttack);
	combatComponent->OnLandAttack.AddDynamic(this, &ACombatPawn::LandAttack);
	movementComponent->SetHalfHeight(collisionCapsule->GetScaledCapsuleHalfHeight());
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

	if (movementComponent->IsInHitlag()) {
		skeletalMesh->bPauseAnims = true;
	}
	else {
		skeletalMesh->bPauseAnims = false;
	}
}

void ACombatPawn::HitByAttack(UHurtbox* hitCollider, UHitbox* attackingCollider, FHitResult hitResult) {
	if (bRecieveKnockback) {
		movementComponent->ApplyKnockback(combatComponent->GetKnockbackVector(hitCollider, attackingCollider));
		movementComponent->ApplyHitlag(hitlagLength);
	}
}

void ACombatPawn::LandAttack(UHitbox* attackingCollider, UHurtbox* hitCollider, FHitResult hitResult) {
	movementComponent->ApplyHitlag(hitlagLength);
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

void ACombatPawn::CancelAttack() {
	if (IsAttacking())
		skeletalMesh->GetAnimInstance()->Montage_Stop(0.05f, GetCurrentMontage());
}