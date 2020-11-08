// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SUActionEffect.h"
#include "SUAttributeComponent.h"
#include "SUActionEffect_Thorns.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API USUActionEffect_Thorns : public USUActionEffect
{
	GENERATED_BODY()
	
public: 
	USUActionEffect_Thorns();

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USUAttributeComponent* OwningComp, float NewHealth, float Delta);

	void StartAction_Implementation(AActor* Instigator) override;

	void StopAction_Implementation(AActor* Instigator) override;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	float ThornsMod;

};
