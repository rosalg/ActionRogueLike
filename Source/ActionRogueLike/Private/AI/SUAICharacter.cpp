// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SUAICharacter.h"
#include "Perception/PawnSensingComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "SUAttributeComponent.h"
#include "BrainComponent.h"
#include "SUWorldUserWidget.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SUActionComponent.h"

// Sets default values
ASUAICharacter::ASUAICharacter()
{
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");

	AttributeComp = CreateDefaultSubobject<USUAttributeComponent>("AttributeComp");

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);

	ActionComp = CreateDefaultSubobject<USUActionComponent>("ActionComp");

	TimeToHitParamName = "TimeToHit";
	TargetActorKey = "TargetActor";
}

/*
* Initialize any event listeners.
*/
void ASUAICharacter::PostInitializeComponents() {
	Super::PostInitializeComponents();

	PawnSensingComp->OnSeePawn.AddDynamic(this, &ASUAICharacter::OnPawnSeen);
	AttributeComp->OnHealthChanged.AddDynamic(this, &ASUAICharacter::OnHealthChanged);
}


/***************************************
Getters/Setters
****************************************/

/*
* Gets the target actor.
* @return Returns the AActor* that the SUAICharacter is targeting.
*/
AActor* ASUAICharacter::GetTargetActor() const {
	AAIController* AIC = Cast<AAIController>(GetController());
	if (AIC) {
		return Cast<AActor>(AIC->GetBlackboardComponent()->GetValueAsObject(TargetActorKey));
	}
	return nullptr;
}

/*
* Sets the target of the SUAICharacter.
* @param NewTarget - The target to be shot at.
* @return None
*/
void ASUAICharacter::SetTargetActor(AActor* NewTarget) {
	AAIController* AIC = Cast<AAIController>(GetController());
	if (AIC) {
		AIC->GetBlackboardComponent()->SetValueAsObject(TargetActorKey, NewTarget);
	}
}

/***************************************
Events
****************************************/

/*
* 
* This event handles the pawn's health changing by updating the pawn's target to be the damage dealer, creating a
* health bar if necessary, making it hitflash, and handling death.
* 
* @param InstigatorActor - Actor the caused the health change.
* @param OwningComp - Component of Actor to make changes to.
* @param NewHealth - The health after changes have been made.
* @param Delta - The amount the health changed by.
* @return None.
*/
void ASUAICharacter::OnHealthChanged(AActor* InstigatorActor, USUAttributeComponent* OwningComp, float NewHealth, float Delta) {
	if (Delta < 0.0f) {

		if (InstigatorActor != this) {
			SetTargetActor(InstigatorActor);
		}
		if (ActiveHealthBar == nullptr) {
			ActiveHealthBar = CreateWidget<USUWorldUserWidget>(GetWorld(), HealthBarWidgetClass);
			if (ActiveHealthBar) {
				ActiveHealthBar->AttachedActor = this;
				ActiveHealthBar->AddToViewport();
			}
		}

		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->TimeSeconds);

		if (NewHealth <= 0.0f) {
			// Stop BT
			AAIController* AIC = Cast<AAIController>(GetController());
			if (AIC) {
				AIC->GetBrainComponent()->StopLogic("Killed");
			}

			// Ragdoll
			GetMesh()->SetAllBodiesSimulatePhysics(true);
			GetMesh()->SetCollisionProfileName("Ragdoll");

			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			GetCharacterMovement()->DisableMovement();

			// set lifespan
			SetLifeSpan(10.0f);
		}
	}
}

/*
* This function handles the logic for when the SUAICharacter sees the player.
* It is implemented using pawn sensing. Currently, it updates the target of the
* SUAICharacter then adds a small notification to the Viewport.
* 
* @param Pawn - The pawn that we saw.
* @return None
*/
void ASUAICharacter::OnPawnSeen(APawn* Pawn) {
	if (GetTargetActor() != Pawn) {
		SetTargetActor(Pawn);

		USUWorldUserWidget* NewWidget = CreateWidget<USUWorldUserWidget>(GetWorld(), SpottedWidgetClass);
		if (NewWidget) {
			NewWidget->AttachedActor = this;

			NewWidget->AddToViewport(10);
		}
	}
}