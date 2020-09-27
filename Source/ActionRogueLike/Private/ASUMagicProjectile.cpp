// Fill out your copyright notice in the Description page of Project Settings.


#include "ASUMagicProjectile.h"


// Sets default values
AASUMagicProjectile::AASUMagicProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}

// Called when the game starts or when spawned
void AASUMagicProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AASUMagicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

