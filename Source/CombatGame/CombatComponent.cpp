#include "CombatComponent.h"

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();
}

FVector UCombatComponent::GetKnockbackVector(UHurtbox* hitCollider, UHitbox* attackingCollider){
	FVector horizontalVector = hitCollider->GetOwner()->GetActorLocation() - attackingCollider->GetOwner()->GetActorLocation();
	horizontalVector = horizontalVector.GetClampedToMaxSize(1.0f) * attackingCollider->GetHorizontalKnockback();
	FVector verticalVector = FVector(0.0f, 0.0f, attackingCollider->GetVerticalKnockback());
	return horizontalVector + verticalVector;
}

UHitbox* UCombatComponent::SpawnHitbox(USceneComponent* parentComponent, FName socket, FVector offset, FRotator rotation, float length, float width, FName hitGroup, float damage, float horizontalKnocback, float verticalKnockback) {
	UHitbox* spawnedHitbox = NewObject<UHitbox>(this, UHitbox::StaticClass());
	spawnedHitbox->AttachTo(parentComponent, socket);
	spawnedHitbox->SetAbsolute(false, false, true);
	spawnedHitbox->AddLocalOffset(offset/parentComponent->GetSocketTransform(socket).GetScale3D());
	spawnedHitbox->AddLocalRotation(rotation);
	spawnedHitbox->SetCapsuleSize(width, length, true);
	spawnedHitbox->SetHitGroup(hitGroup);
	spawnedHitbox->SetDamage(damage);
	spawnedHitbox->SetKnockback(horizontalKnocback, verticalKnockback);
	spawnedHitbox->RegisterComponent();

	return spawnedHitbox;
}