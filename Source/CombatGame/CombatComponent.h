#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatCollider.h"
#include "CombatComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FLandAttackDelegate, UCombatCollider*, attackingCollider, UCombatCollider*, hitCollider, FHitResult, hitResult);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FHitByAttackDelegate, UCombatCollider*, hitCollider, UCombatCollider*, attackingCollider, FHitResult, hitResult);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FSuccesfulGuardDelegate, UCombatCollider*, guardingCollider, UCombatCollider*, attackingCollider, FHitResult, hitResult);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FAttackGuardedDelegate, UCombatCollider*, attackingCollider, UCombatCollider*, guardingCollider, FHitResult, hitResult);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FAttackClankedDelegate, UCombatCollider*, attackingCollider, UCombatCollider*, otherAttackingCollider, FHitResult, hitResult);

UCLASS( BlueprintType, meta=(BlueprintSpawnableComponent), DisplayName = "Combat Component" )
class COMBATGAME_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCombatComponent();

protected:
	virtual void BeginPlay() override;	

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
		FVector GetKnockbackVector(UCombatCollider* hitCollider, UCombatCollider* attackingCollider);

	UFUNCTION(BlueprintCallable)
		void SpawnCombatCollider(FName socketName, FVector offset, float length, float width, FName attackGroup, ECombatColliderType type, bool isIntangible, float damage, float horizontalKnocback, float verticalKnockback);
};
