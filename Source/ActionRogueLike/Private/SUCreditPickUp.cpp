// Fill out your copyright notice in the Description page of Project Settings.


#include "SUCreditPickUp.h"
#include "SUAttributeComponent.h"
#include "Components/StaticMeshComponent.h"
#include "SUPlayerState.h"
#include "Net/UnrealNetwork.h"

ASUCreditPickUp::ASUCreditPickUp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	CreditCost = 0;
	PickUpValue = 5;
}

// Called when the game starts or when spawned
void ASUCreditPickUp::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASUCreditPickUp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


/*
* This function responds to the player's interaction event, grabbing the player's attribute component, applying a positive health change, then
* disabling the health potion for 10 seconds.
*
* PARAMETERS
*	APawn* Instigator Pawn - The pawn that invoked the interact implementation.
* RETURN
*	None
*/
void ASUCreditPickUp::Interact_Implementation(APawn* InstigatorPawn) {
	bInteractable = false;
	OnRep_PickUpStateChange();

	ASUPlayerState* State = Cast<ASUPlayerState>(InstigatorPawn->GetPlayerState());
	State->UpdateCredits(InstigatorPawn, PickUpValue);

	GetWorldTimerManager().SetTimer(Cooldown_TimerHandle, this, &ASUPickUpBase::Reset_Interactability, InteractionCooldown);
}

