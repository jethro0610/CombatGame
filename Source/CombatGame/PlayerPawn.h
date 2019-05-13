#pragma once

#include "CoreMinimal.h"
#include "CombatPawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "PlayerPawn.generated.h"

USTRUCT(BlueprintType)
struct FAttackGroup {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
		FName attackGroupName;

	UPROPERTY(EditAnywhere)
		TArray<UAnimMontage*> starters;

	UPROPERTY(EditAnywhere)
		TArray<UAnimMontage*> finishers;
};

UCLASS()
class COMBATGAME_API APlayerPawn : public ACombatPawn
{
	GENERATED_BODY()
public:
	APlayerPawn();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
		USpringArmComponent* cameraSpringArm;

	UPROPERTY(VisibleAnywhere)
		UCameraComponent* camera;

	UPROPERTY(EditAnywhere)
		int maxStarterAttacks = 2;

	UPROPERTY(EditAnywhere)
		int maxFinisherAttacks = 1;

	UPROPERTY(EditAnywhere)
		TArray<FAttackGroup> attackGroups;

	UPROPERTY(EditAnywhere)
		float maxAttackDistSpeed;

	UPROPERTY(EditAnywhere)
		float maxTargetDistance;

	TWeakObjectPtr<ACombatPawn> currentTarget;

	int starterAttackCount;
	int finisherAttackCount;
	bool bCanCombo;

	float moveXInput;
	float moveYInput;

	float cameraYaw;
	float cameraPitch;

public:
	UFUNCTION(BlueprintCallable)
		FAttackGroup GetAttackGroupFromName(FName attackGroupName);

	UFUNCTION(BlueprintCallable)
		void DoAttackFromGroup(FName attackGroupName);

	UFUNCTION(BlueprintCallable)
		void EnableCombo();

	UFUNCTION(BlueprintCallable)
		void ResetCombo();

	UFUNCTION(BlueprintCallable)
		ACombatPawn* GetNearestCombatPawn();

private:
	void InputMoveX(float axisValue);
	void InputMoveY(float axisValue);
	void InputCameraX(float axisValue);
	void InputCameraY(float axisValue);
	void InputJump();
	void InputReleaseJump();
	void InputAttack();

	UFUNCTION()
		void PlayerEnterGround();

	UFUNCTION()
		void PlayerLeaveGround();
};
