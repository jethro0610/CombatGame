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

void UCombatComponent::SpawnCombatCollider(FName socketName, FVector offset, float length, float width, FName attackGroup, ECombatColliderType type, bool isIntangible, float damage, float horizontalKnocback, float verticalKnockback) {
	UCombatCollider* spawnedCollider = NewObject<UCombatCollider>(UCombatCollider::StaticClass());
	spawnedCollider->AttachTo(GetOwner()->GetRootComponent(), socketName);
	spawnedCollider->AddLocalOffset(offset);

	spawnedCollider->SetCapsuleSize(width, length, true);
	spawnedCollider->SetAttackGroup(attackGroup);
	spawnedCollider->SetCombatColliderType(type);
	spawnedCollider->SetIntangible(isIntangible);
	spawnedCollider->SetDamage(damage);
	spawnedCollider->SetKnockback(horizontalKnocback, verticalKnockback);
}
