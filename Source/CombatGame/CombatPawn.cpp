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

void ACombatPawn::OnConstruction(const FTransform &Transform) {
	// Rotate the mesh properly
	skeletalMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -collisionCapsule->GetUnscaledCapsuleHalfHeight()));
}

void ACombatPawn::BeginPlay()
{
	Super::BeginPlay();

	GetMovement()->SetDesiredRotation(GetActorRotation());

	combatComponent->OnHitByAttack.AddDynamic(this, &ACombatPawn::HitByAttack);
	combatComponent->OnLandAttack.AddDynamic(this, &ACombatPawn::LandAttack);
	movementComponent->SetHalfHeight(collisionCapsule->GetScaledCapsuleHalfHeight());

	modelOffset = skeletalMesh->GetComponentLocation() - GetActorLocation();
}

void ACombatPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Move with animation root motion
	if (bApplyRootMotion && movementComponent != nullptr && skeletalMesh != nullptr) {
		FTransform rootTransform = skeletalMesh->ConsumeRootMotion().GetRootMotionTransform();
		movementComponent->Move(skeletalMesh->GetComponentRotation().RotateVector(rootTransform.GetLocation()));
	}

	// Reset the attack montage
	if (currentAttackMontage != nullptr && GetCurrentMontage() == nullptr)
		currentAttackMontage = nullptr;

	// Slow down animation in hitlag
	if (GetCurrentMontage() != nullptr) {
		if (movementComponent->IsInHitlag()) {
			GetCurrentMontage()->RateScale = animationHitlagSpeed;
		}
		else {
			GetCurrentMontage()->RateScale = 1.0f;
		}
	}

	// Interpolate mesh location
	skeletalMesh->SetWorldLocation(movementComponent->GetInterpolatedPosition() + modelOffset);
}

bool ACombatPawn::SetActorRotation(FRotator newRotator, ETeleportType teleportType) {
	movementComponent->SetDesiredRotation(newRotator);
	return Super::SetActorRotation(newRotator, teleportType);
}

void ACombatPawn::HitByAttack(UHurtbox* hitCollider, UHitbox* attackingCollider, FHitResult hitResult) {
	if (bRecieveKnockback) {
		// Give knockback when hit
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
	if (skeletalMesh != nullptr && skeletalMesh->GetAnimInstance() != nullptr) {
		return skeletalMesh->GetAnimInstance()->GetCurrentActiveMontage();
	}
	else {
		return nullptr;
	}
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
	// Play attack animation
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