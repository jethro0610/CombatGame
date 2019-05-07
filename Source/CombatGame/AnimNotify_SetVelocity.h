#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_SetVelocity.generated.h"

/**
 * 
 */
UCLASS()
class COMBATGAME_API UAnimNotify_SetVelocity : public UAnimNotify
{
	GENERATED_BODY()
	
private:
	virtual void Notify(USkeletalMeshComponent* meshComponent, UAnimSequenceBase* animSequence) override;

public:
	UPROPERTY(EditAnywhere)
		float forwardVelocity;

	UPROPERTY(EditAnywhere)
		float verticalVelocity;
};
