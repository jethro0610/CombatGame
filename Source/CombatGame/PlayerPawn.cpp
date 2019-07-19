#include "PlayerPawn.h"
#include "EngineUtils.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/InputComponent.h"
#include "Components/SkeletalMeshComponent.h"

APlayerPawn::APlayerPawn() {
	FAttackGroup groundAttacks;
	groundAttacks.attackGroupName = FName("Ground Attacks");
	attackGroups.Add(groundAttacks);

	FAttackGroup airAttacks;
	airAttacks.attackGroupName = FName("Air Attacks");
	attackGroups.Add(airAttacks);

	photoComponent = CreateDefaultSubobject<UPhotoComponent>("Photo Component");

	cameraSpringArm = CreateDefaultSubobject<USpringArmComponent>("Spring Arm");
	cameraSpringArm->bInheritYaw = false;
	cameraSpringArm->bInheritPitch = false;
	cameraSpringArm->bInheritRoll = false;
	cameraSpringArm->bAbsoluteRotation = true;
	cameraSpringArm->SetWorldRotation(FRotator(0.0f, 0.0f, 0.0f));	// These three are needed to keep the camera
	cameraYaw = cameraSpringArm->GetComponentRotation().Yaw;		// spring arm rotated in the editor, otherwise
	cameraPitch = cameraSpringArm->GetComponentRotation().Pitch;	// it stays on the same world rotation.

	camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	camera->AttachTo(cameraSpringArm);
}

void APlayerPawn::BeginPlay() {
	Super::BeginPlay();

	cameraYaw = cameraSpringArm->GetComponentRotation().Yaw;
	cameraPitch = cameraSpringArm->GetComponentRotation().Pitch;

	GetMovement()->OnEnterGround.AddDynamic(this, &APlayerPawn::PlayerEnterGround);
	GetMovement()->OnLeaveGround.AddDynamic(this, &APlayerPawn::PlayerLeaveGround);
}

void APlayerPawn::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	cameraSpringArm->SetRelativeRotation(FRotator(cameraPitch, cameraYaw, 0.0f));
	cameraSpringArm->SetWorldLocation(movementComponent->GetInterpolatedPosition());

	if (!IsAttacking() && cameraSpringArm != nullptr) {
		if (!bPhotoMode) {
			if (moveYInput != 0.0f || moveXInput != 0.0f) {
				FVector cameraForwardVector = FRotator(0.0f, cameraYaw, 0.0f).Vector();			// Yaw value must be taken from variable because the
				FVector cameraRightVector = FRotator(0.0f, cameraYaw + 90.0f, 0.0f).Vector();	// GetComponentRotation yaw value is affected by parent rotation

				FVector forwardMoveDirection = moveYInput * cameraForwardVector;
				FVector rightMoveDirection = moveXInput * cameraRightVector;
				FVector moveDirection = forwardMoveDirection + rightMoveDirection;
				moveDirection = FVector(moveDirection.X, moveDirection.Y, 0.0f);
				GetMovement()->SetDesiredMovement(moveDirection, FMath::Min(moveDirection.Size(), 1.0f));

				FVector directionVector;
				if (GetMovement()->IsOnGround()) {
					directionVector = moveDirection;
				}
				else {
					directionVector = GetMovement()->GetVelocityNoGravity();
				}
				GetMovement()->SetDesiredRotation(directionVector.Rotation());
			}
		}
		else {
			camera->SetWorldLocation(movementComponent->GetInterpolatedPosition());
			SetActorRotation(FRotator(0.0f, cameraYaw, 0.0f));
			GetMovement()->SetDesiredRotation(FRotator(0.0f, cameraYaw, 0.0f));
			camera->SetRelativeRotation(FRotator(cameraPitch, 0.0f, 0.0f));

			FVector cameraForwardVector = FRotator(0.0f, cameraYaw, 0.0f).Vector();			// Yaw value must be taken from variable because the
			FVector cameraRightVector = FRotator(0.0f, cameraYaw + 90.0f, 0.0f).Vector();	// GetComponentRotation yaw value is affected by parent rotation

			FVector forwardMoveDirection = moveYInput * cameraForwardVector;
			FVector rightMoveDirection = moveXInput * cameraRightVector;
			FVector moveDirection = forwardMoveDirection + rightMoveDirection;
			moveDirection = FVector(moveDirection.X, moveDirection.Y, 0.0f);
			GetMovement()->SetDesiredMovement(moveDirection, FMath::Min(moveDirection.Size(), 1.0f));
		}
	}
	GetMovement()->SetMoveInAir(!IsAttacking());

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

void APlayerPawn::TogglePhotoMode(bool onOff) {
	if (bPhotoMode != onOff && !IsAttacking()) {
		bPhotoMode = onOff;
		cameraPitch = 0.0f;
		if (bPhotoMode) {
			camera->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetIncludingScale);
			skeletalMesh->SetVisibility(false);
		}
		else {
			camera->AttachTo(cameraSpringArm);
			camera->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
			skeletalMesh->SetVisibility(true);
		}
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
	PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Released, this, &APlayerPawn::InputReleaseJump);
	PlayerInputComponent->BindAction("Attack", EInputEvent::IE_Pressed, this, &APlayerPawn::InputAttack);
	PlayerInputComponent->BindAction("TogglePhotoMode", EInputEvent::IE_Pressed, this, &APlayerPawn::InputEnablePhotoMode);
	PlayerInputComponent->BindAction("TogglePhotoMode", EInputEvent::IE_Released, this, &APlayerPawn::InputDisablePhotoMode);
	PlayerInputComponent->BindAction("TakePhoto", EInputEvent::IE_Pressed, this, &APlayerPawn::InputTakePhoto);
}

void APlayerPawn::InputMoveX(float axisValue) {
	moveXInput = axisValue;
}

void APlayerPawn::InputMoveY(float axisValue) {
	moveYInput = axisValue;
}

void APlayerPawn::InputCameraX(float axisValue) {
	cameraYaw += axisValue * 120.0f * GetWorld()->DeltaTimeSeconds;
}

void APlayerPawn::InputCameraY(float axisValue) {
	cameraPitch += axisValue * 120.0f * GetWorld()->DeltaTimeSeconds;
	cameraPitch = FMath::Clamp(cameraPitch, -89.0f, 89.0f);
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
	if (!bPhotoMode) {
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
}

void APlayerPawn::InputEnablePhotoMode() {
	TogglePhotoMode(true);
}

void APlayerPawn::InputDisablePhotoMode() {
	TogglePhotoMode(false);
}

void APlayerPawn::InputTakePhoto() {
	photoComponent->TakePhoto();
}

void APlayerPawn::PlayerEnterGround() {
	CancelAttack();
	ResetCombo();
}

void APlayerPawn::PlayerLeaveGround() {
	CancelAttack();
	ResetCombo();
}