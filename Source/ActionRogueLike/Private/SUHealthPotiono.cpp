// Fill out your copyright notice in the Description page of Project Settings.


#include "SUHealthPotiono.h"
#include "SUAttributeComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ASUHealthPotiono::ASUHealthPotiono()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent = MeshComp;

	Interactable = true;
	HealAmount = 20.0f;
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
void ASUHealthPotiono::Interact_Implementation(APawn* InstigatorPawn) {
	USUAttributeComponent* AttributeComp = Cast<USUAttributeComponent>(InstigatorPawn->GetComponentByClass(USUAttributeComponent::StaticClass()));
	if (Interactable && AttributeComp->ApplyHealthChange(HealAmount)) {
		Interactable = false;
		MeshComp->SetVisibility(false);
		SetActorEnableCollision(false);
		GetWorldTimerManager().SetTimer(Cooldown, this, &ASUHealthPotiono::Reset_Interactability, 10.0f);
	}
}

/*
* This function should only happen 10 seconds after a user tried using this health potion. 
* 
* It will make it so the health potion can be used again.
*/
void ASUHealthPotiono::Reset_Interactability() {
	Interactable = true;
	MeshComp->SetVisibility(true);
	SetActorEnableCollision(true);
}

// Called when the game starts or when spawned
void ASUHealthPotiono::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASUHealthPotiono::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

