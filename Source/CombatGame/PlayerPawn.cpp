#include "PlayerPawn.h"
#include "EngineUtils.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/InputComponent.h"

APlayerPawn::APlayerPawn() {
	FAttackGroup groundAttacks;
	groundAttacks.attackGroupName = FName("Ground Attacks");
	attackGroups.Add(groundAttacks);

	FAttackGroup airAttacks;
	airAttacks.attackGroupName = FName("Air Attacks");
	attackGroups.Add(airAttacks);

	cameraSpringArm = CreateDefaultSubobject<USpringArmComponent>("Spring Arm");
	cameraSpringArm->bInheritYaw = false;
	cameraSpringArm->bInheritPitch = false;
	cameraSpringArm->bInheritRoll = false;
	cameraSpringArm->bAbsoluteRotation = true;
	cameraSpringArm->AttachTo(RootComponent);

	camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	camera->AttachTo(cameraSpringArm);
}

void APlayerPawn::BeginPlay() {
	Super::BeginPlay();

	GetMovement()->OnEnterGround.AddDynamic(this, &APlayerPawn::PlayerEnterGround);
	GetMovement()->OnLeaveGround.AddDynamic(this, &APlayerPawn::PlayerLeaveGround);
}

void APlayerPawn::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (!IsAttacking() && cameraSpringArm != nullptr) {
		if (moveYInput != 0.0f || moveXInput != 0.0f) {
			FVector cameraForwardVector = FRotator(0.0f, camera->GetComponentRotation().Yaw, 0.0f).Vector();
			FVector cameraRightVector = FRotator(0.0f, camera->GetComponentRotation().Yaw + 90.0f, 0.0f).Vector();

			FVector forwardWalkDirection = moveYInput * cameraForwardVector;
			FVector rightWalkDirection = moveXInput * cameraRightVector;
			FVector walkDirection = forwardWalkDirection + rightWalkDirection;
			walkDirection = FVector(walkDirection.X, walkDirection.Y, 0.0f);
			GetMovement()->Walk(walkDirection, FMath::Min(walkDirection.Size(), 1.0f));
			
			FVector directionVector;
			if (GetMovement()->IsOnGround()) {
				directionVector = walkDirection;
			}
			else {
				directionVector = GetMovement()->GetVelocityNoGravity();
			}
			FRotator lerpRotator = FQuat::Slerp(GetActorRotation().Quaternion(), directionVector.ToOrientationQuat(), 1.0f - FMath::Exp(-12.0f * DeltaTime)).Rotator();
			SetActorRotation(lerpRotator);
		}
	}

	GetMovement()->SetMoveInAir(!IsAttacking());

	cameraSpringArm->SetWorldLocation(movementComponent->GetInterpolatedPosition());
	currentTarget = GetNearestCombatPawn();
}

void APlayerPawn::EnableCombo() {
	if (starterAttackCount < maxStarterAttacks) {
		starterAttackCount += 1;
	}
	else if(finisherAttackCount < maxFinisherAttacks){
		finisherAttackCount += 1;
	}
	bCanCombo = true;
}

void APlayerPawn::ResetCombo() {
	starterAttackCount = 0;
	finisherAttackCount = 0;
	bCanCombo = false;
}

FAttackGroup APlayerPawn::GetAttackGroupFromName(FName attackGroupName) {
	FAttackGroup returnGroup;
	for (int i = 0; i < attackGroups.Num(); i++)
	{
		if (attackGroups[i].attackGroupName == attackGroupName) {
			returnGroup = attackGroups[i];
		}
	}
	return returnGroup;
}

void APlayerPawn::DoAttackFromGroup(FName attackGroupName) {
	FAttackGroup currentAttackGroup = GetAttackGroupFromName(attackGroupName);
	if (!IsAttacking() || bCanCombo) {
		if (starterAttackCount < maxStarterAttacks && currentAttackGroup.starters.Num() > 0) {
			int attackIndex = starterAttackCount % currentAttackGroup.starters.Num();
			DoAttackMontage(currentAttackGroup.starters[attackIndex], true);
		}
		else if (finisherAttackCount < maxFinisherAttacks && currentAttackGroup.finishers.Num() > 0) {
			int attackIndex = finisherAttackCount % currentAttackGroup.finishers.Num();
			DoAttackMontage(currentAttackGroup.finishers[attackIndex], true);
		}
		bCanCombo = false;
	}
}

ACombatPawn* APlayerPawn::GetNearestCombatPawn() {
	float dist = -1.0f;
	ACombatPawn* returnPawn = nullptr;
	for (TActorIterator<ACombatPawn> actorItr(GetWorld()); actorItr; ++actorItr) {
		ACombatPawn* currentPawn = *actorItr;
		if (currentPawn != this) {
			float currentDist = FVector::Dist(GetActorLocation(), currentPawn->GetActorLocation());
			if (currentDist <= dist || dist == -1.0f) {
				returnPawn = currentPawn;
				dist = currentDist;
			}
		}
	}
	return returnPawn;
}

void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveX", this, &APlayerPawn::InputMoveX);
	PlayerInputComponent->BindAxis("MoveY", this, &APlayerPawn::InputMoveY);
	PlayerInputComponent->BindAxis("CameraX", this, &APlayerPawn::InputCameraX);
	PlayerInputComponent->BindAxis("CameraY", this, &APlayerPawn::InputCameraY);
	PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &APlayerPawn::InputJump);
	PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Released, this, &APlayerPawn::InputReleaseJump);
	PlayerInputComponent->BindAction("Attack", EInputEvent::IE_Pressed, this, &APlayerPawn::InputAttack);
}

void APlayerPawn::InputMoveX(float axisValue) {
	moveXInput = axisValue;
}

void APlayerPawn::InputMoveY(float axisValue) {
	moveYInput = axisValue;
}

void APlayerPawn::InputCameraX(float axisValue) {
	float newCameraXRotation = cameraSpringArm->GetComponentRotation().Yaw + (axisValue * 120.0f * GetWorld()->DeltaTimeSeconds);
	FRotator newCameraRotation = FRotator(cameraSpringArm->GetComponentRotation().Pitch, newCameraXRotation, 0.0f);
	cameraSpringArm->SetWorldRotation(newCameraRotation);
}

void APlayerPawn::InputCameraY(float axisValue) {
	float newCameraYRotation = cameraSpringArm->GetComponentRotation().Pitch + (axisValue * 120.0f * GetWorld()->DeltaTimeSeconds);
	newCameraYRotation = FMath::Clamp(newCameraYRotation, -90.0f, 90.0f);
	FRotator newCameraRotation = FRotator(newCameraYRotation, cameraSpringArm->GetComponentRotation().Yaw, 0.0f);
	cameraSpringArm->SetWorldRotation(newCameraRotation);
}

void APlayerPawn::InputJump() {
	GetMovement()->Jump();
}

void APlayerPawn::InputReleaseJump() {
	if (GetMovement()->GetGravity() < 0.0f) {
		GetMovement()->SetGravity(GetMovement()->GetGravity() / 3.0f);
	}
}

void APlayerPawn::InputAttack() {
	if (currentTarget != nullptr) {
		if (!IsAttacking() || bCanCombo) {
			if (FVector::Dist(GetActorLocation(), currentTarget->GetActorLocation()) < maxTargetDistance) {
				FRotator lookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), currentTarget->GetActorLocation() + (currentTarget->GetMovement()->GetVelocity() * 5.0f));
				SetActorRotation(FRotator(0.0f, lookAtRotation.Yaw, 0.0f));
			}
		}
	}
	if (GetMovement()->IsOnGround()) {
		DoAttackFromGroup("Ground Attacks");
	}
	else {
		DoAttackFromGroup("Air Attacks");
	}
}

void APlayerPawn::PlayerEnterGround() {
	CancelAttack();
	ResetCombo();
}

void APlayerPawn::PlayerLeaveGround() {
	CancelAttack();
	ResetCombo();
}