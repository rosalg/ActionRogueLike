// Fill out your copyright notice in the Description page of Project Settings.


#include "SUHealthPotion.h"
#include "SUAttributeComponent.h"
#include "Components/StaticMeshComponent.h"
#include "SUPlayerState.h"

ASUHealthPotion::ASUHealthPotion()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	CreditCost = 3;
	HealAmount = 20.0f;
}

// Called when the game starts or when spawned
void ASUHealthPotion::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASUHealthPotion::Tick(float DeltaTime)
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
void ASUHealthPotion::Interact_Implementation(APawn* InstigatorPawn) {
	ASUPlayerState* State = Cast<ASUPlayerState>(InstigatorPawn->GetPlayerState());
	USUAttributeComponent* AttributeComp = Cast<USUAttributeComponent>(InstigatorPawn->GetComponentByClass(USUAttributeComponent::StaticClass()));
	if (State->GetNumCredits() >= CreditCost && AttributeComp->ApplyHealthChange(this, HealAmount)) {
		bInteractable = false;
		OnRep_PickUpStateChange();

		State->UpdateCredits(InstigatorPawn, -CreditCost);

		GetWorldTimerManager().SetTimer(Cooldown_TimerHandle, this, &ASUPickUpBase::Reset_Interactability, InteractionCooldown);
	}
}

