#include "Hitbox.h"
#include "CombatComponent.h"
#include "Hurtbox.h"

UHitbox::UHitbox() {
	OnComponentBeginOverlap.AddDynamic(this, &UHitbox::OnBeginOverlap);
	SetCollisionProfileName("OverlapAll");
	SetVisibility(true);
	SetHiddenInGame(false);
}

void UHitbox::BeginPlay() {
	Super::BeginPlay();
	UpdateCombatComponent();
}

void UHitbox::OnBeginOverlap(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult) {
	if (otherActor != nullptr && otherActor != GetOwner() && otherComp != this && otherComp != nullptr) {
		if (HasCombatComponent()) {
			UHurtbox* connectingHurtbox = Cast<UHurtbox, UPrimitiveComponent>(otherComp);
			if (connectingHurtbox->IsValidLowLevel()) {
				combatComponent->OnLandAttack.Broadcast(this, connectingHurtbox, sweepResult);
			}
		}
	}
}

void UHitbox::UpdateCombatComponent() {
	UCombatComponent* foundComponent = GetOwner()->FindComponentByClass<UCombatComponent>();
	if (foundComponent != nullptr && foundComponent->IsValidLowLevel())
		combatComponent = foundComponent;
}

bool UHitbox::HasCombatComponent() {
	return (combatComponent != nullptr && combatComponent->IsValidLowLevel());
}

FName UHitbox::GetHitGroup() {
	return hitGroup;
}

void UHitbox::SetHitGroup(FName newGroup) {
	hitGroup = newGroup;
}

float UHitbox::GetDamage() {
	return damage;
}

void UHitbox::SetDamage(float newDamage) {
	damage = newDamage;
}

float UHitbox::GetHorizontalKnockback() {
	return horizontalKnockback;
}

float UHitbox::GetVerticalKnockback() {
	return verticalKnockback;
}

void UHitbox::SetKnockback(float newHorizontalKnockback, float newVerticalKnockback) {
	horizontalKnockback = newHorizontalKnockback;
	verticalKnockback = newVerticalKnockback;
}