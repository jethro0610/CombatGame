#include "CombatComponent.h"

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();
}

FVector UCombatComponent::GetKnockbackVector(UCombatCollider* hitCollider, UCombatCollider* attackingCollider){
	FVector horizontalVector = hitCollider->GetOwner()->GetActorLocation() - attackingCollider->GetOwner()->GetActorLocation();
	horizontalVector = horizontalVector.GetClampedToMaxSize(1.0f) * attackingCollider->GetHorizontalKnockback();
	FVector verticalVector = FVector(0.0f, 0.0f, attackingCollider->GetVerticalKnockback());
	return horizontalVector + verticalVector;
}
