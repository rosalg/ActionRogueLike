// Fill out your copyright notice in the Description page of Project Settings.


#include "SUItemChest.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"
#include "..\Public\SUItemChest.h"


// Sets default values
ASUItemChest::ASUItemChest()
{
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent = BaseMesh;

	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LidMesh"));
	LidMesh->SetupAttachment(BaseMesh);

	TargetPitch = 110;

	// Says to Unreal to consider actor and update clients to have same values.
	SetReplicates(true);
}

void ASUItemChest::Interact_Implementation(APawn* InstigatorPawn) {

	bLidOpened = !bLidOpened;
	// Need to manually call to server
	OnRep_LidOpened();
}

void ASUItemChest::OnActorLoaded_Implementation()
{
	OnRep_LidOpened();
}

void ASUItemChest::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// When bLidOpened has changed, send it to all clients.
	DOREPLIFETIME(ASUItemChest, bLidOpened);
}

// Automatically called to client.
void ASUItemChest::OnRep_LidOpened()
{
	float CurrPitch = bLidOpened ? TargetPitch : 0.0f;
	LidMesh->SetRelativeRotation(FRotator(CurrPitch, 0, 0));
}


