// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SUPickUpBase.h"
#include "SUHealthPotion.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASUHealthPotion : public ASUPickUpBase
{
	GENERATED_BODY()
public:

	void Interact_Implementation(APawn* InstigatorPawn);

	ASUHealthPotion();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:

	UPROPERTY(EditAnywhere, Category = "Heal Amount")
	float HealAmount;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
