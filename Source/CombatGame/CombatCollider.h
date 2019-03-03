#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "CombatCollider.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FLandAttackDelegate, UCombatCollider*, attackingCollider, UCombatCollider*, hitCollider, FHitResult, hitResult);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FHitByAttackDelegate, UCombatCollider*, hitCollider, UCombatCollider*, attackingCollider, FHitResult, hitResult);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FSuccesfulGuardDelegate, UCombatCollider*, guardingCollider, UCombatCollider*, attackingCollider, FHitResult, hitResult);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FAttackGuardedDelegate, UCombatCollider*, attackingCollider, UCombatCollider*, guardingCollider, FHitResult, hitResult);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FAttackClankedDelegate, UCombatCollider*, attackingCollider, UCombatCollider*, otherAttackingCollider, FHitResult, hitResult);

UENUM(BlueprintType)
enum class ECombatColliderType : uint8 {
	Attack	UMETA(DisplayName = "Attack"),
	Hurt	UMETA(DisplayName = "Hurt"),
	Guard	UMETA(DisplayName = "Guard")
};

UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class COMBATGAME_API UCombatCollider : public UCapsuleComponent
{
	GENERATED_BODY()

public:
	UCombatCollider();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
		FName attackGroup;

	UPROPERTY(EditAnywhere)
		ECombatColliderType combatColliderType;

	UPROPERTY(EditAnywhere)
		bool bIntangible;

	UPROPERTY(EditAnywhere)
		float damage;

	UPROPERTY(EditAnywhere)
		float horizontalKnockback;

	UPROPERTY(EditAnywhere)
		float verticalKnockback;

	UFUNCTION()
		void OnBeginOverlap(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult);

public:
	UPROPERTY(BlueprintAssignable)
		FLandAttackDelegate OnLandAttack;

	UPROPERTY(BlueprintAssignable)
		FHitByAttackDelegate OnHitByAttack;

	UPROPERTY(BlueprintAssignable)
		FSuccesfulGuardDelegate OnSuccesfulGuard;

	UPROPERTY(BlueprintAssignable)
		FAttackGuardedDelegate OnAttackGuarded;

	UPROPERTY(BlueprintAssignable)
		FAttackClankedDelegate OnAttackClanked;

	UFUNCTION(BlueprintCallable)
		ECombatColliderType GetCombatColliderType();

	UFUNCTION(BlueprintCallable)
		void SetCombatColliderType(ECombatColliderType newType);

	UFUNCTION(BlueprintCallable)
		bool IsIntangible();

	UFUNCTION(BlueprintCallable)
		void SetIntangible(bool isIntangible);
};
