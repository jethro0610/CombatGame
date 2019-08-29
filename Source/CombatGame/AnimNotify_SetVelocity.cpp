#include "AnimNotify_SetVelocity.h"
#include "CombatPawn.h"

void UAnimNotify_SetVelocity::Notify(USkeletalMeshComponent* meshComponent, UAnimSequenceBase* animSequence) {
	// Cast to CombatPawn
	TWeakObjectPtr<ACombatPawn> owningPawn = Cast<ACombatPawn, AActor>(meshComponent->GetOwner());

	if (owningPawn != nullptr) {
		// Apply velocity
		FVector forwardVector = owningPawn->GetActorForwardVector() * forwardVelocity;
		FVector verticalVector = owningPawn->GetActorUpVector() * verticalVelocity;
		owningPawn->GetMovement()->SetVelocity(forwardVector + verticalVector);
	}
}