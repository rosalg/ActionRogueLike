// Fill out your copyright notice in the Description page of Project Settings.


#include "SUAttributeComponent.h"
#include "Math/UnrealMathUtility.h"

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
	Health = FMath::Clamp(Health, 0.0f, MaxHealth);
	if (OldHealth == Health) return false;
	OnHealthChanged.Broadcast(InstigatorActor, this, Health, Health - OldHealth);

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
