// Fill out your copyright notice in the Description page of Project Settings.


#include "SUAttributeComponent.h"
#include "Math/UnrealMathUtility.h"
#include <SUGameModeBase.h>
#include "Net/UnrealNetwork.h"
#include "..\Public\SUAttributeComponent.h"

static TAutoConsoleVariable<float> CVarDamageMultiplier(TEXT("su.DamageMultiplier"), 1.0f, TEXT("Global Damage Mod for Attri Comp"), ECVF_Cheat);

// Sets default values for this component's properties
USUAttributeComponent::USUAttributeComponent()
{

	Health = 100;
	MaxHealth = 100;
	Rage = 0;
	MaxRage = 100;
	DamageModifier = 0;
	bProjectileCanBurn = false;

	SetIsReplicatedByDefault(true);
}

float USUAttributeComponent::GetCurrentHealth() {
	return Health;
}

float USUAttributeComponent::GetCurrentDamageModifier()
{
	return DamageModifier;
}

void USUAttributeComponent::MulticastHealthChanged_Implementation(AActor* InstigatorActor, float NewHealth, float Delta)
{
	OnHealthChanged.Broadcast(InstigatorActor, this, NewHealth, Delta);
}

void USUAttributeComponent::MulticastRageChanged_Implementation(AActor* InstigatorActor, float NewRage, float Delta)
{
	OnRageChange.Broadcast(InstigatorActor, this, NewRage, Delta);
}

bool USUAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta) {
	/*if (!GetOwner()->HasAuthority()) {
		// This ensures only our server can apply health changes. Then, if the server hears about a client health change
		// it'll make the health change to its copy of the client. Then because health is a replicated variable,
		// every other client will make its corresponding update.

		// Not good enough though, because this will prevent any logic dependent on the bool that this function returns
		// (like explosions)
		return false;
	}*/
	
	// For god mode that we didn't implement
	if (!GetOwner()->CanBeDamaged() && Delta < 0.0f) {
		return false;
	}

	if (Delta < 0.0f) {
		ApplyRageChange(InstigatorActor, -Delta * 2);
		float DamageMultiplier = CVarDamageMultiplier.GetValueOnGameThread();
		Delta *= DamageMultiplier;
	}

	float OldHealth = Health;
	float NewHealth = FMath::Clamp(Health + Delta, 0.0f, MaxHealth);
	float ActualDelta = NewHealth - OldHealth;

	if (GetOwner()->HasAuthority()) {

		Health = NewHealth;

		if (ActualDelta != 0.0f) {
			MulticastHealthChanged(InstigatorActor, Health, ActualDelta);
		}

		if (ActualDelta < 0.0f && Health == 0.0f) {
			ASUGameModeBase* GM = GetWorld()->GetAuthGameMode<ASUGameModeBase>();
			if (GM) {
				GM->OnActorKilled(GetOwner(), InstigatorActor);
			}
		}

	}

	return ActualDelta != 0;
}

bool USUAttributeComponent::ApplyRageChange(AActor* InstigatorActor, float Delta)
{
	float OldRage = Rage;
	Rage += Delta;
	Rage = FMath::Clamp(Rage, 0.0f, MaxRage);
	float ActualDelta = Rage - OldRage;
	if (ActualDelta == 0) return false;
	//OnRageChange.Broadcast(InstigatorActor, this, Rage, ActualDelta);
	// Multiplayer
	MulticastRageChanged(InstigatorActor, Rage, Delta);
	return true;
}

USUAttributeComponent* USUAttributeComponent::GetAttributes(AActor* FromActor) {
	if (FromActor) {
		return  Cast<USUAttributeComponent>(FromActor->GetComponentByClass(USUAttributeComponent::StaticClass()));
	}

	return nullptr;
}

bool USUAttributeComponent::ApplyMaxHealthChange(AActor* InstigatorActor, float Delta) {
	MaxHealth += Delta;
	return true;
}

bool USUAttributeComponent::ApplyDamageModifierChange(AActor* InstigatorActor, float Delta) {
	DamageModifier += Delta;
	return true;
}

bool USUAttributeComponent::IsAlive(AActor* Actor) {
	USUAttributeComponent* AttributeComp = GetAttributes(Actor);
	if (AttributeComp) {
		return AttributeComp->GetCurrentHealth() > 0.0f;
	}
	return false;
}

float USUAttributeComponent::GetMaxHealth()
{
	return MaxHealth;
}

bool USUAttributeComponent::Kill(AActor* InstigatorActor) {
	return ApplyHealthChange(InstigatorActor, -MaxHealth);
}

void USUAttributeComponent::SetProjectileCanBurn(bool CanBurn) {
	bProjectileCanBurn = CanBurn;
}

void USUAttributeComponent::SetCanReflect(bool CanReflect)
{
	bCanReflect = CanReflect;
}

void USUAttributeComponent::SetCanJump(bool CanJump)
{
	bCanJump = CanJump;
}

void USUAttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USUAttributeComponent, Health);
	DOREPLIFETIME(USUAttributeComponent, MaxHealth);
	DOREPLIFETIME(USUAttributeComponent, Rage);
	DOREPLIFETIME(USUAttributeComponent, MaxRage);
	DOREPLIFETIME(USUAttributeComponent, bProjectileCanBurn);
	DOREPLIFETIME(USUAttributeComponent, DamageModifier);

	// This is just for optimization for bandwidth to minimize how much we need to send over.
	// DOREPLIFETIME_CONDITION(USUAttributeComponent, HealthMax, COND_OwnerOnly);
}
