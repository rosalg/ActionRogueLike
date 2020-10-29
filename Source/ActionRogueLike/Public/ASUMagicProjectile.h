// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SUBaseProjectile.h"
#include "GameplayTagContainer.h"
#include "ASUMagicProjectile.generated.h"

class UParticleSystemComponent;
class USoundCue;
class UParticleSystem;
class USUActionEffect;

UCLASS()
class ACTIONROGUELIKE_API AASUMagicProjectile : public ASUBaseProjectile
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AASUMagicProjectile();

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	FGameplayTag ParryTag;

	UPROPERTY(EditAnywhere, Category = "Attack")
	float Damage;

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	TSubclassOf<USUActionEffect> BurningActionClass;

	UFUNCTION()
	void OnActorOverlap(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, Category = "Attack")
	USoundCue* ImpactSound;

	UPROPERTY(EditAnywhere, Category = "Attack")
	UParticleSystem* ImpactParticle;

};
