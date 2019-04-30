#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "CombatCollider.generated.h"

UENUM(BlueprintType)
enum class ECombatColliderType : uint8 {
	Attack	UMETA(DisplayName = "Attack"),
	Hurt	UMETA(DisplayName = "Hurt"),
	Guard	UMETA(DisplayName = "Guard")
};

class UCombatComponent;

UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class COMBATGAME_API UCombatCollider : public UCapsuleComponent
{
	GENERATED_BODY()

public:
	UCombatCollider();

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UCombatComponent* combatComponent;

	bool HasCombatComponent();

	UPROPERTY(EditAnywhere)
		ECombatColliderType combatColliderType;

	UPROPERTY(EditAnywhere)
		bool bIntangible;

	UPROPERTY(EditAnywhere)
		FName attackGroup;

	UPROPERTY(EditAnywhere)
		float damage;

	UPROPERTY(EditAnywhere)
		float horizontalKnockback;

	UPROPERTY(EditAnywhere)
		float verticalKnockback;

	UFUNCTION()
		void OnBeginOverlap(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult);

public:
	UFUNCTION(BlueprintCallable)
		ECombatColliderType GetCombatColliderType();

	UFUNCTION(BlueprintCallable)
		void SetCombatColliderType(ECombatColliderType newType);

	UFUNCTION(BlueprintCallable)
		bool IsIntangible();

	UFUNCTION(Blueprintcallable)
		FName GetAttackGroup();

	UFUNCTION(BlueprintCallable)
		void SetAttackGroup(FName newGroup);

	UFUNCTION(BlueprintCallable)
		float GetDamage();

	UFUNCTION(BlueprintCallable)
		void SetDamage(float newDamage);

	UFUNCTION(BlueprintCallable)
		void SetIntangible(bool isIntangible);

	UFUNCTION(BlueprintCallable)
		float GetHorizontalKnockback();

	UFUNCTION(BlueprintCallable)
		float GetVerticalKnockback();

	UFUNCTION(BlueprintCallable)
		void SetKnockback(float newHorizontalKnockback, float newVerticalKnockback);
};
