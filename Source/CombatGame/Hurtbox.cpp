#include "Hurtbox.h"
#include "CombatComponent.h"
#include "Hitbox.h"

UHurtbox::UHurtbox() {
	OnComponentBeginOverlap.AddDynamic(this, &UHurtbox::OnBeginOverlap);
	SetCollisionProfileName("OverlapAll");
}

void UHurtbox::BeginPlay()
{
	Super::BeginPlay();
	UpdateCombatComponent();
}

void UHurtbox::OnBeginOverlap(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult) {
	if (otherActor != nullptr && otherActor != GetOwner() && otherComp != this && otherComp != nullptr) {
		if (HasCombatComponent()) {
			UHitbox* connectingHitbox = Cast<UHitbox, UPrimitiveComponent>(otherComp);
			if (connectingHitbox != nullptr) {
				if (!combatComponent->HasContactedHitbox(connectingHitbox)) {
					combatComponent->OnHitByAttack.Broadcast(this, connectingHitbox, sweepResult);
					combatComponent->AddContactedHitbox(connectingHitbox);
					connectingHitbox->OnDeleteHitbox.AddDynamic(combatComponent.Get(), &UCombatComponent::OnContactedHitboxDeleted);
				}
			}
		}
	}
}

void UHurtbox::SetInvincible(bool isInvincible) {
	bInvincible = isInvincible;
}

bool UHurtbox::IsInvincible() {
	return bInvincible;
}

void UHurtbox::UpdateCombatComponent() {
	UCombatComponent* foundComponent = GetOwner()->FindComponentByClass<UCombatComponent>();
	if (foundComponent != nullptr)
		combatComponent = foundComponent;
}

bool UHurtbox::HasCombatComponent() {
	return (combatComponent != nullptr);
}