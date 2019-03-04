#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "CombatComponent.h"
#include "CombatCollider.generated.h"

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
	UCombatComponent* combatComponent;

	bool HasCombatComponent();

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
	UFUNCTION(BlueprintCallable)
		ECombatColliderType GetCombatColliderType();

	UFUNCTION(BlueprintCallable)
		void SetCombatColliderType(ECombatColliderType newType);

	UFUNCTION(BlueprintCallable)
		bool IsIntangible();

	UFUNCTION(BlueprintCallable)
		void SetIntangible(bool isIntangible);

	UFUNCTION(BlueprintCallable)
		float GetHorizontalKnockback();

	UFUNCTION(BlueprintCallable)
		float GetVerticalKnockback();
};
