// Fill out your copyright notice in the Description page of Project Settings.


#include "SUAttributeComponent.h"
#include "Math/UnrealMathUtility.h"
#include <SUGameModeBase.h>
#include "Net/UnrealNetwork.h"

static TAutoConsoleVariable<float> CVarDamageMultiplier(TEXT("su.DamageMultiplier"), 1.0f, TEXT("Global Damage Mod for Attri Comp"), ECVF_Cheat);

// Sets default values for this component's properties
USUAttributeComponent::USUAttributeComponent()
{

	Health = 100;
	MaxHealth = 100;
	Rage = 0;
	MaxRage = 100;

	SetIsReplicatedByDefault(true);
}

float USUAttributeComponent::GetCurrentHealth() {
	return Health;
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
	float OldHealth = Health;
	if (Delta < 0.0f) {
		ApplyRageChange(InstigatorActor, -Delta * 2);
		float DamageMultiplier = CVarDamageMultiplier.GetValueOnGameThread();
		Delta *= DamageMultiplier;
	}
	Health += Delta;
	Health = FMath::Clamp(Health, 0.0f, MaxHealth);
	float ActualDelta = Health - OldHealth;
	if (ActualDelta == 0) return false;

	// We actually had a health change.
	//OnHealthChanged.Broadcast(InstigatorActor, this, Health, ActualDelta);

	// For Multiplayer
	MulticastHealthChanged(InstigatorActor, Health, ActualDelta);

	if (ActualDelta < 0.0f && Health == 0.0f) {
		ASUGameModeBase* GM = GetWorld()->GetAuthGameMode<ASUGameModeBase>();
		if (GM) {
			GM->OnActorKilled(GetOwner(), InstigatorActor);
		}
	}

	return true;
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

void USUAttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USUAttributeComponent, Health);
	DOREPLIFETIME(USUAttributeComponent, MaxHealth);
	DOREPLIFETIME(USUAttributeComponent, Rage);
	DOREPLIFETIME(USUAttributeComponent, MaxRage);

	// This is just for optimization for bandwidth to minimize how much we need to send over.
	// DOREPLIFETIME_CONDITION(USUAttributeComponent, HealthMax, COND_OwnerOnly);
}
