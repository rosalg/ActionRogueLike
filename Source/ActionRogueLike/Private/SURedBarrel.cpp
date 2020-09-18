// Fill out your copyright notice in the Description page of Project Settings.


#include "SURedBarrel.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Components/StaticMeshComponent.h"



// Sets default values
ASURedBarrel::ASURedBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	RootComponent = MeshComp;

	RadialForceComp = CreateDefaultSubobject<URadialForceComponent>("RadialForceComp");
	RadialForceComp->SetupAttachment(MeshComp);

}

// Called when the game starts or when spawned
void ASURedBarrel::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASURedBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

