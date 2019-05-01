#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "Hurtbox.generated.h"

class UCombatComponent;

UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class COMBATGAME_API UHurtbox : public UCapsuleComponent
{
	GENERATED_BODY()
public:
	UHurtbox();

protected:
	virtual void BeginPlay() override;

private:
	bool bInvincible;
	UCombatComponent* combatComponent;

	UFUNCTION()
		void OnBeginOverlap(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult);

public:
	UFUNCTION(BlueprintCallable)
		void SetInvincible(bool isInvincible);

	UFUNCTION(BlueprintCallable)
		bool IsInvincible();

	UFUNCTION(BlueprintCallable)
		void UpdateCombatComponent();

	UFUNCTION(BlueprintCallable)
		bool HasCombatComponent();
};
