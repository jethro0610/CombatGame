#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "Hitbox.generated.h"

class UCombatComponent;

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

	UFUNCTION()
		void OnBeginOverlap(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult);

public:
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
