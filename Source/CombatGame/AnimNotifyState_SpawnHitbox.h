#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Hitbox.h"
#include "Components/CapsuleComponent.h"
#include "AnimNotifyState_SpawnHitbox.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName = "Spawn Hitbox"))
class COMBATGAME_API UAnimNotifyState_SpawnHitbox : public UAnimNotifyState, public FTickableGameObject
{
	GENERATED_BODY()
	
public:
	UAnimNotifyState_SpawnHitbox();
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComponent, UAnimSequenceBase* AnimationSequence, float TotalDuration);
	virtual void NotifyTick(USkeletalMeshComponent* MeshComponent, UAnimSequenceBase* AnimationSequence, float FrameDeltaTime);
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComponent, UAnimSequenceBase* AnimationSequence);

public:
	virtual void Tick(float DeltaTime) override;
	virtual bool IsTickable() const override;
	virtual bool IsTickableInEditor() const override;
	virtual bool IsTickableWhenPaused() const override;
	virtual TStatId GetStatId() const override;

public:
	UPROPERTY(EditAnywhere)
		FName socket;

	UPROPERTY(EditAnywhere)
		FVector offset;

	UPROPERTY(EditAnywhere)
		FRotator rotation;

	UPROPERTY(EditAnywhere)
		float length;

	UPROPERTY(EditAnywhere)
		float width;

	UPROPERTY(EditAnywhere)
		FName hitGroup;

	UPROPERTY(EditAnywhere)
		float damage;

	UPROPERTY(EditAnywhere)
		float horizontalKnockback;

	UPROPERTY(EditAnywhere)
		float verticalKnockback;

private:
	USkeletalMeshComponent* currentMeshComponent;
	UAnimMontage* currentAnimation;
	float notifyStartTime;
	float notifyEndTime;
	UHitbox* spawnedHitbox;
};
