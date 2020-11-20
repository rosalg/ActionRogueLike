// Fill out your copyright notice in the Description page of Project Settings.


#include "SUActionEffect_Thorns.h"
#include "SUGameplayFunctionLibrary.h"
#include "SUAttributeComponent.h"
#include "SUActionComponent.h"
#include "SUCharacter.h"

USUActionEffect_Thorns::USUActionEffect_Thorns() {
	Duration = 0;
	Period = 0;
	ThornsMod = 0.5;
}

void USUActionEffect_Thorns::OnHealthChanged(AActor* InstigatorActor, USUAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	USUAttributeComponent* DamageDealerAttributeComp = USUAttributeComponent::GetAttributes(InstigatorActor);
	AActor* ThornsOwner = GetOwningComponent()->GetOwner();
	USUAttributeComponent* DamageTakerAttributeComp = USUAttributeComponent::GetAttributes(ThornsOwner);
	// If the thorns owner isn't the same person dealing the damage
	if (DamageTakerAttributeComp) {
		if (DamageTakerAttributeComp->bCanReflect && ThornsOwner != InstigatorActor && Delta < 0.0f) {
			int32 ReflectedAmount = FMath::Abs(FMath::RoundToInt(Delta * ThornsMod));
			UE_LOG(LogTemp, Warning, TEXT("Checking... %d"), ReflectedAmount);
			if (ReflectedAmount == 0) return;
			//Reflect damage
			USUGameplayFunctionLibrary::ApplyDamage(ThornsOwner, InstigatorActor, ReflectedAmount);
		}
	}
	
}

void USUActionEffect_Thorns::StartAction_Implementation(AActor* Instigator) {
	Super::StartAction_Implementation(Instigator);
	USUAttributeComponent::GetAttributes(Instigator)->OnHealthChanged.AddDynamic(this, &USUActionEffect_Thorns::OnHealthChanged);
}

void USUActionEffect_Thorns::StopAction_Implementation(AActor* Instigator) {
	Super::StopAction_Implementation(Instigator);
	USUAttributeComponent::GetAttributes(Instigator)->OnHealthChanged.RemoveDynamic(this, &USUActionEffect_Thorns::OnHealthChanged);
}
