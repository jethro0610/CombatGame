#include "CombatCollider.h"

UCombatCollider::UCombatCollider() {
	SetCollisionProfileName("OverlapAll");

	OnComponentBeginOverlap.AddDynamic(this, &UCombatCollider::OnBeginOverlap);
}

void UCombatCollider::BeginPlay()
{
	Super::BeginPlay();
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

float UCombatCollider::GetHorizontalKnockback() {
	return horizontalKnockback;
}

float UCombatCollider::GetVerticalKnockback() {
	return verticalKnockback;
}

void UCombatCollider::OnBeginOverlap(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult)
{
	if (otherActor!= nullptr && otherActor != GetOwner() && otherComp != this && otherComp != nullptr) {
		UCombatCollider* otherCombatCollider = Cast<UCombatCollider>(otherComp);
		if (otherCombatCollider->IsValidLowLevel()) {
			if (GetCombatColliderType() == ECombatColliderType::Attack && otherCombatCollider->GetCombatColliderType() == ECombatColliderType::Hurt && !otherCombatCollider->IsIntangible()) {
				OnLandAttack.Broadcast(this, otherCombatCollider, sweepResult);
			}

			if (GetCombatColliderType() == ECombatColliderType::Hurt && otherCombatCollider->GetCombatColliderType() == ECombatColliderType::Attack && !IsIntangible()) {
				OnHitByAttack.Broadcast(this, otherCombatCollider, sweepResult);
			}

			if (GetCombatColliderType() == ECombatColliderType::Guard && otherCombatCollider->GetCombatColliderType() == ECombatColliderType::Attack) {
				OnSuccesfulGuard.Broadcast(this, otherCombatCollider, sweepResult);
			}

			if (GetCombatColliderType() == ECombatColliderType::Attack && otherCombatCollider->GetCombatColliderType() == ECombatColliderType::Guard) {
				OnAttackGuarded.Broadcast(this, otherCombatCollider, sweepResult);
			}

			if (GetCombatColliderType() == ECombatColliderType::Attack && otherCombatCollider->GetCombatColliderType() == ECombatColliderType::Attack) {
				OnAttackClanked.Broadcast(this, otherCombatCollider, sweepResult);
			}
		}
	}
}