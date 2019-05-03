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
	
}

void ACombatPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bApplyRootMotion && movementComponent->IsValidLowLevel() && skeletalMesh->IsValidLowLevel()) {
		FTransform rootTransform = skeletalMesh->ConsumeRootMotion().GetRootMotionTransform();
		movementComponent->Move(skeletalMesh->GetComponentRotation().RotateVector(rootTransform.GetLocation()));
	}
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

void ACombatPawn::PlayAnimMontage(UAnimMontage* animMontage) {
	UAnimInstance* animInstance = skeletalMesh->GetAnimInstance();

	if(animInstance->IsValidLowLevel())
		animInstance->Montage_Play(animMontage);
}

UAnimMontage* ACombatPawn::GetCurrentMontage() {
	UAnimInstance* animInstance = skeletalMesh->GetAnimInstance();

	return animInstance->GetCurrentActiveMontage();
}

bool ACombatPawn::MontageIsAttack(UAnimMontage* inMontage) {
	bool returnValue = false;
	for (int i = 0; i < attacks.Num(); i++) {
		if (attacks[i].attackComboOrder.Contains(inMontage)) {
			returnValue = true;
			break;
		}
	}
	return returnValue;
}

bool ACombatPawn::IsAttacking() {
	if (GetCurrentMontage()->IsValidLowLevel()) {
		if (MontageIsAttack(GetCurrentMontage())) {
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
 
int ACombatPawn::GetAttackIndexFromName(FName attackName) {
	int returnAttack = -1;
	for (int i = 0; i < attacks.Num(); i++) {
		if (attacks[i].attackName == attackName) {
			returnAttack = i;
			break;
		}
	}
	return returnAttack;
}

void ACombatPawn::DoAttack(FName attackName) {
	FAttack attack = attacks[GetAttackIndexFromName(attackName)];
	if (!IsAttacking()) {
		PlayAnimMontage(attack.attackComboOrder[0]);
		OnAttack.Broadcast();
	}
	else if(canCombo && currentComboLength <= attack.attackComboOrder.Num() - 1){
		canCombo = false;
		PlayAnimMontage(attack.attackComboOrder[currentComboLength]);
		OnAttack.Broadcast();
	}
}

void ACombatPawn::EnableCombo() {
	canCombo = true;
	currentComboLength += 1;
}

void ACombatPawn::ResetCombo() {
	canCombo = false;
	currentComboLength = 0;
}