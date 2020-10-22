// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SUPickUpBase.h"
#include "SUCreditPickUp.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASUCreditPickUp : public ASUPickUpBase
{
	GENERATED_BODY()
public:

	void Interact_Implementation(APawn* InstigatorPawn);

	ASUCreditPickUp();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:

	UPROPERTY(EditAnywhere, Category = "Variable Nums")
	float PickUpValue;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
