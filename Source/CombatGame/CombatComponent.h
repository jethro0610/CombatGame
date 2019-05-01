#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Hitbox.h"
#include "Hurtbox.h"
#include "CombatComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FLandAttackDelegate, UHitbox*, attackingCollider, UHurtbox*, hitCollider, FHitResult, hitResult);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FHitByAttackDelegate, UHurtbox*, hitCollider, UHitbox*, attackingCollider, FHitResult, hitResult);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FSuccesfulGuardDelegate, UHurtbox*, guardingCollider, UHitbox*, attackingCollider, FHitResult, hitResult);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FAttackGuardedDelegate, UHitbox*, attackingCollider, UHurtbox*, guardingCollider, FHitResult, hitResult);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FAttackClankedDelegate, UHitbox*, attackingCollider, UHitbox*, otherAttackingCollider, FHitResult, hitResult);

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
		FVector GetKnockbackVector(UHurtbox* hitCollider, UHitbox* attackingCollider);

	UFUNCTION(BlueprintCallable)
		UHitbox* SpawnHitbox(USceneComponent* parentComponent, FName socket, FVector offset, FRotator rotation, float length, float width, FName hitGroup, float damage, float horizontalKnocback, float verticalKnockback);
};
