#include "CombatComponent.h"

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCombatComponent::AddContactedHitbox(UHitbox* hitbox) {
	contactedHitboxes.Add(hitbox);
}

void UCombatComponent::OnContactedHitboxDeleted(UHitbox* hitboxDeleted) {
	contactedHitboxes.Remove(hitboxDeleted);
}

bool UCombatComponent::HasContactedHitbox(UHitbox* hitbox) {
	return contactedHitboxes.Contains(hitbox);
}

FVector UCombatComponent::GetKnockbackVector(UHurtbox* hitCollider, UHitbox* attackingCollider){
	// Calculate knockback vector
	FVector horizontalVector = hitCollider->GetOwner()->GetActorLocation() - attackingCollider->GetOwner()->GetActorLocation();
	horizontalVector = horizontalVector.GetClampedToSize(1.0f, 1.0f) * attackingCollider->GetHorizontalKnockback();
	horizontalVector = FVector(horizontalVector.X, horizontalVector.Y, 0.0f);
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
	spawnedHitbox->RegisterComponent(); // Add the component to the world

	return spawnedHitbox;
}