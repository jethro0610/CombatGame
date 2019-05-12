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
	skeletalMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -collisionCapsule->GetUnscaledCapsuleHalfHeight()));
}

void ACombatPawn::BeginPlay()
{
	Super::BeginPlay();
	combatComponent->OnHitByAttack.AddDynamic(this, &ACombatPawn::HitByAttack);
	combatComponent->OnLandAttack.AddDynamic(this, &ACombatPawn::LandAttack);
	movementComponent->SetHalfHeight(collisionCapsule->GetScaledCapsuleHalfHeight());

	modelOffset = skeletalMesh->GetComponentLocation() - GetActorLocation();
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

	if (GetCurrentMontage() != nullptr) {
		if (movementComponent->IsInHitlag()) {
			GetCurrentMontage()->RateScale = animationHitlagSpeed;
		}
		else {
			GetCurrentMontage()->RateScale = 1.0f;
		}
	}

	skeletalMesh->SetWorldLocation(movementComponent->GetInterpolatedPosition() + modelOffset);
}

bool ACombatPawn::SetActorRotation(FRotator NewRotatation) {
	GetMovement()->SetDesiredRotation(NewRotatation);
	return Super::SetActorRotation(NewRotatation);
}

bool ACombatPawn::SetActorRotation(FRotator NewRotatation, ETeleportType Teleport) {
	GetMovement()->SetDesiredRotation(NewRotatation);
	return Super::SetActorRotation(NewRotatation, Teleport);
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