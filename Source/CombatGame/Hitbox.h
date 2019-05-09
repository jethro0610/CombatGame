#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "Hitbox.generated.h"

class UHurtbox;
class UCombatComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDeleteHitboxDelegate, UHitbox*, hitboxDestroyed);

UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class COMBATGAME_API UHitbox : public UCapsuleComponent
{
	GENERATED_BODY()
		
public:
	UHitbox();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
		FName hitGroup;

	UPROPERTY(EditAnywhere)
		float damage;

	UPROPERTY(EditAnywhere)
		float horizontalKnockback;

	UPROPERTY(EditAnywhere)
		float verticalKnockback;

	TWeakObjectPtr<UCombatComponent> combatComponent;

	TArray<TWeakObjectPtr<UHurtbox>> contactedHurtboxes;

	UFUNCTION()
		void OnBeginOverlap(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult);

public:
	UPROPERTY(BlueprintAssignable)
		FDeleteHitboxDelegate OnDeleteHitbox;

	UFUNCTION(BlueprintCallable)
		void DeleteHitbox();

	UFUNCTION(Blueprintcallable)
		void UpdateCombatComponent();

	UFUNCTION(Blueprintcallable)
		bool HasCombatComponent();

	UFUNCTION(Blueprintcallable)
		FName GetHitGroup();

	UFUNCTION(BlueprintCallable)
		void SetHitGroup(FName newGroup);

	UFUNCTION(BlueprintCallable)
		float GetDamage();

	UFUNCTION(BlueprintCallable)
		void SetDamage(float newDamage);

	UFUNCTION(BlueprintCallable)
		float GetHorizontalKnockback();

	UFUNCTION(BlueprintCallable)
		float GetVerticalKnockback();

	UFUNCTION(BlueprintCallable)
		void SetKnockback(float newHorizontalKnockback, float newVerticalKnockback);
};
