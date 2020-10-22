// Fill out your copyright notice in the Description page of Project Settings.


#include "SUPickUpBase.h"


// Sets default values
ASUPickUpBase::ASUPickUpBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent = MeshComp;

	Interactable = true;
	InteractionCooldown = 10.0f;
	CreditCost = 1;
}

/*
* This function should only happen 10 seconds after a user tried using this health potion.
*
* It will make it so the pick up can be used again.
*/
void ASUPickUpBase::Reset_Interactability() {
	Interactable = true;
	MeshComp->SetVisibility(true);
	SetActorEnableCollision(true);
}

// Called when the game starts or when spawned
void ASUPickUpBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASUPickUpBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

