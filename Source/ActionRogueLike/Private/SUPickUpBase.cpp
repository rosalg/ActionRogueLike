// Fill out your copyright notice in the Description page of Project Settings.


#include "SUPickUpBase.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ASUPickUpBase::ASUPickUpBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent = MeshComp;

	bInteractable = true;
	InteractionCooldown = 10.0f;
	CreditCost = 1;

	SetReplicates(true);
}

/*
* This function should only happen 10 seconds after a user tried using this health potion.
*
* It will make it so the pick up can be used again.
*/
void ASUPickUpBase::Reset_Interactability() {
	bInteractable = true;
	OnRep_PickUpStateChange();
}

void ASUPickUpBase::OnRep_PickUpStateChange() {
	MeshComp->SetVisibility(bInteractable);
	SetActorEnableCollision(bInteractable);
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


void ASUPickUpBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// When bLidOpened has changed, send it to all clients.
	DOREPLIFETIME(ASUPickUpBase, bInteractable);
}
