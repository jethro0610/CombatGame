#include "PlayerPawn.h"
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
			FVector forwardWalkDirection = moveYInput * camera->GetForwardVector();
			FVector rightWalkDirection = moveXInput * camera->GetRightVector();
			FVector walkDirection = forwardWalkDirection + rightWalkDirection;
			walkDirection = FVector(walkDirection.X, walkDirection.Y, 0.0f);
			GetMovement()->Walk(walkDirection, walkDirection.Size());
			SetActorRotation(walkDirection.ToOrientationRotator());
		}
	}
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

void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveX", this, &APlayerPawn::InputMoveX);
	PlayerInputComponent->BindAxis("MoveY", this, &APlayerPawn::InputMoveY);
	PlayerInputComponent->BindAxis("CameraX", this, &APlayerPawn::InputCameraX);
	PlayerInputComponent->BindAxis("CameraY", this, &APlayerPawn::InputCameraY);
	PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &APlayerPawn::InputJump);
	PlayerInputComponent->BindAction("Attack", EInputEvent::IE_Pressed, this, &APlayerPawn::InputAttack);
}

void APlayerPawn::InputMoveX(float axisValue) {
	moveXInput = axisValue;
}

void APlayerPawn::InputMoveY(float axisValue) {
	moveYInput = axisValue;
}

void APlayerPawn::InputCameraX(float axisValue) {
	float newCameraXRotation = cameraSpringArm->GetComponentRotation().Yaw + axisValue;
	FRotator newCameraRotation = FRotator(cameraSpringArm->GetComponentRotation().Pitch, newCameraXRotation, 0.0f);
	cameraSpringArm->SetWorldRotation(newCameraRotation);
}

void APlayerPawn::InputCameraY(float axisValue) {
	float newCameraYRotation = cameraSpringArm->GetComponentRotation().Pitch + axisValue;
	newCameraYRotation = FMath::Clamp(newCameraYRotation, -90.0f, 90.0f);
	FRotator newCameraRotation = FRotator(newCameraYRotation, cameraSpringArm->GetComponentRotation().Yaw, 0.0f);
	cameraSpringArm->SetWorldRotation(newCameraRotation);
}

void APlayerPawn::InputJump() {
	GetMovement()->Jump();
}

void APlayerPawn::InputAttack() {
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