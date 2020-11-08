// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SUGameplayInterface.h"
#include "SUPickUpBase.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ASUPickUpBase : public AActor, public ISUGameplayInterface
{
	GENERATED_BODY()
	
public:

	UFUNCTION()
	void Reset_Interactability();

protected:

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere)
	float InteractionCooldown;

	UPROPERTY(EditAnywhere)
	int CreditCost;

	FTimerHandle Cooldown_TimerHandle;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnRep_PickUpStateChange();

	UPROPERTY(ReplicatedUsing = "OnRep_PickUpStateChange")
	bool bInteractable;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Sets default values for this actor's properties
	ASUPickUpBase();


};
