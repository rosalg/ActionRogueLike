// Fill out your copyright notice in the Description page of Project Settings.


#include "SUActionEffect_Thorns.h"
#include "SUGameplayFunctionLibrary.h"
#include "SUAttributeComponent.h"
#include "SUActionComponent.h"

USUActionEffect_Thorns::USUActionEffect_Thorns() {
	Duration = 0;
	Period = 0;
	ThornsMod = 0.5;
}

void USUActionEffect_Thorns::OnHealthChanged(AActor* InstigatorActor, USUAttributeComponent* OwningComp, float NewHealth, float Delta, float NewRage)
{
	USUAttributeComponent* DamageDealerAttributeComp = USUAttributeComponent::GetAttributes(InstigatorActor);
	AActor* ThornsOwner = GetOwningComponent()->GetOwner();
	// If the thorns owner isn't the same person dealing the damage
	if (ThornsOwner != InstigatorActor) {
		//Reflect damage
		USUGameplayFunctionLibrary::ApplyDamage(ThornsOwner, InstigatorActor, -Delta * ThornsMod);
	}
}

void USUActionEffect_Thorns::StartAction_Implementation(AActor* Instigator) {
	Super::StartAction_Implementation(Instigator);
	UE_LOG(LogTemp, Log, TEXT("Binding!"));
	USUAttributeComponent::GetAttributes(Instigator)->OnHealthChanged.AddDynamic(this, &USUActionEffect_Thorns::OnHealthChanged);
}

void USUActionEffect_Thorns::StopAction_Implementation(AActor* Instigator) {
	Super::StopAction_Implementation(Instigator);
	USUAttributeComponent::GetAttributes(Instigator)->OnHealthChanged.RemoveDynamic(this, &USUActionEffect_Thorns::OnHealthChanged);
}
