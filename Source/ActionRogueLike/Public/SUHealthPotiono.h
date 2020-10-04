// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SUGameplayInterface.h"
#include "SUHealthPotiono.generated.h"

class UStaticMeshComponent;

UCLASS()
class ACTIONROGUELIKE_API ASUHealthPotiono : public AActor, public ISUGameplayInterface
{
	GENERATED_BODY()
	
public:	
	
	void Interact_Implementation(APawn* InstigatorPawn);

	bool Interactable;

protected:

	UPROPERTY(EditAnywhere, Category = "Heal Amount")
	float HealAmount;

	FTimerHandle Cooldown;

	void Reset_Interactability();

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComp;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Sets default values for this actor's properties
	ASUHealthPotiono();

};
