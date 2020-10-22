// Fill out your copyright notice in the Description page of Project Settings.


#include "SUAttributeComponent.h"
#include "Math/UnrealMathUtility.h"
#include <SUGameModeBase.h>

static TAutoConsoleVariable<float> CVarDamageMultiplier(TEXT("su.DamageMultiplier"), 1.0f, TEXT("Global Damage Mod for Attri Comp"), ECVF_Cheat);

// Sets default values for this component's properties
USUAttributeComponent::USUAttributeComponent()
{

	Health = 100;
	MaxHealth = 100;

}

float USUAttributeComponent::GetCurrentHealth() {
	return Health;
}

bool USUAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta) {
	float OldHealth = Health;
	Health += Delta;
	if (Delta < 0.0f) {
		float DamageMultiplier = CVarDamageMultiplier.GetValueOnGameThread();

		Delta *= DamageMultiplier;
	}

	Health = FMath::Clamp(Health, 0.0f, MaxHealth);
	float ActualDelta = Health - OldHealth;
	if (ActualDelta == 0) return false;
	OnHealthChanged.Broadcast(InstigatorActor, this, Health, ActualDelta);

	if (ActualDelta < 0.0f && Health == 0.0f) {
		ASUGameModeBase* GM = GetWorld()->GetAuthGameMode<ASUGameModeBase>();
		if (GM) {
			GM->OnActorKilled(GetOwner(), InstigatorActor);
		}
	}

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
