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

UCombatCollider* UCombatComponent::SpawnCombatCollider(USceneComponent* parentComponent, FName socket, FVector offset, FRotator rotation, float length, float width, ECombatColliderType type, bool isIntangible, FName attackGroup, float damage, float horizontalKnocback, float verticalKnockback) {
	UCombatCollider* spawnedCollider = NewObject<UCombatCollider>(this, UCombatCollider::StaticClass());
	spawnedCollider->RegisterComponent();
	spawnedCollider->AttachTo(parentComponent, socket);
	spawnedCollider->SetAbsolute(false, false, true);
	spawnedCollider->AddLocalOffset(offset/parentComponent->GetSocketTransform(socket).GetScale3D());
	spawnedCollider->AddLocalRotation(rotation);
	spawnedCollider->SetCapsuleSize(width, length, true);
	spawnedCollider->SetCombatColliderType(type);
	spawnedCollider->SetIntangible(isIntangible);
	spawnedCollider->SetAttackGroup(attackGroup);
	spawnedCollider->SetDamage(damage);
	spawnedCollider->SetKnockback(horizontalKnocback, verticalKnockback);

	return spawnedCollider;
}