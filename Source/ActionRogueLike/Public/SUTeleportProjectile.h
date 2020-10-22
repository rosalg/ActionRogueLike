// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SUBaseProjectile.h"
#include "SUTeleportProjectile.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ASUTeleportProjectile : public ASUBaseProjectile
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASUTeleportProjectile();
	
	void Teleport();

protected:
	
	UPROPERTY(EditAnywhere, Category = "Detonation")
	UParticleSystem* DetonationEffect;
	
	FTimerHandle Delay;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	UFUNCTION()
	void MaxFlightTime_TimeElapsed();
	
public:
	// Called every frame
	void Detonate();
	
};
