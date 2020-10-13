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

bool USUAttributeComponent::ApplyHealthChange(float Delta) {
	float OldHealth = Health;
	Health += Delta;
	Health = FMath::Clamp(Health, 0.0f, MaxHealth);
	if (OldHealth == Health) return false;
	OnHealthChanged.Broadcast(nullptr, this, Health, Delta);

	return true;
}

