#include "CombatCollider.h"
#include "CombatComponent.h"

UCombatCollider::UCombatCollider() {
	SetCollisionProfileName("OverlapAll");

	OnComponentBeginOverlap.AddDynamic(this, &UCombatCollider::OnBeginOverlap);
}

void UCombatCollider::BeginPlay()
{
	Super::BeginPlay();

	UCombatComponent* foundComponent = GetOwner()->FindComponentByClass<UCombatComponent>();
	if (foundComponent != nullptr && foundComponent->IsValidLowLevel())
		combatComponent = foundComponent;
}

bool UCombatCollider::HasCombatComponent() {
	return (combatComponent != nullptr && combatComponent->IsValidLowLevel());
}

FName UCombatCollider::GetAttackGroup() {
	return attackGroup;
}

void UCombatCollider::SetAttackGroup(FName newGroup) {
	attackGroup = newGroup;
}

ECombatColliderType UCombatCollider::GetCombatColliderType() {
	return combatColliderType;
}

void UCombatCollider::SetCombatColliderType(ECombatColliderType newType) {
	combatColliderType = newType;
}

bool UCombatCollider::IsIntangible() {
	return bIntangible;
}

void UCombatCollider::SetIntangible(bool isIntangible) {
	bIntangible = isIntangible;
}

float UCombatCollider::GetDamage() {
	return damage;
}

void UCombatCollider::SetDamage(float newDamage) {
	damage = newDamage;
}

float UCombatCollider::GetHorizontalKnockback() {
	return horizontalKnockback;
}

float UCombatCollider::GetVerticalKnockback() {
	return verticalKnockback;
}

void UCombatCollider::SetKnockback(float newHorizontalKnockback, float newVerticalKnockback) {
	horizontalKnockback = newHorizontalKnockback;
	verticalKnockback = newVerticalKnockback;
}

void UCombatCollider::OnBeginOverlap(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult)
{
	if (otherActor!= nullptr && otherActor != GetOwner() && otherComp != this && otherComp != nullptr) {
		UCombatCollider* otherCombatCollider = Cast<UCombatCollider>(otherComp);
		if (otherCombatCollider->IsValidLowLevel() && HasCombatComponent()) {
			if (GetCombatColliderType() == ECombatColliderType::Attack && otherCombatCollider->GetCombatColliderType() == ECombatColliderType::Hurt && !otherCombatCollider->IsIntangible()) {
				combatComponent->OnLandAttack.Broadcast(this, otherCombatCollider, sweepResult);
			}

			if (GetCombatColliderType() == ECombatColliderType::Hurt && otherCombatCollider->GetCombatColliderType() == ECombatColliderType::Attack && !IsIntangible()) {
				combatComponent->OnHitByAttack.Broadcast(this, otherCombatCollider, sweepResult);
			}

			if (GetCombatColliderType() == ECombatColliderType::Guard && otherCombatCollider->GetCombatColliderType() == ECombatColliderType::Attack) {
				combatComponent->OnSuccesfulGuard.Broadcast(this, otherCombatCollider, sweepResult);
			}

			if (GetCombatColliderType() == ECombatColliderType::Attack && otherCombatCollider->GetCombatColliderType() == ECombatColliderType::Guard) {
				combatComponent->OnAttackGuarded.Broadcast(this, otherCombatCollider, sweepResult);
			}

			if (GetCombatColliderType() == ECombatColliderType::Attack && otherCombatCollider->GetCombatColliderType() == ECombatColliderType::Attack) {
				combatComponent->OnAttackClanked.Broadcast(this, otherCombatCollider, sweepResult);
			}
		}
	}
}