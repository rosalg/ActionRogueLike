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

void ASUAICharacter::PostInitializeComponents() {
	Super::PostInitializeComponents();

	PawnSensingComp->OnSeePawn.AddDynamic(this, &ASUAICharacter::OnPawnSeen);
	AttributeComp->OnHealthChanged.AddDynamic(this, &ASUAICharacter::OnHealthChanged);
}

AActor* ASUAICharacter::GetTargetActor() const {
	AAIController* AIC = Cast<AAIController>(GetController());
	if (AIC) {
		return Cast<AActor>(AIC->GetBlackboardComponent()->GetValueAsObject(TargetActorKey));
	}
	return nullptr;
}

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

void ASUAICharacter::SetTargetActor(AActor* NewTarget) {
	AAIController* AIC = Cast<AAIController>(GetController());
	if (AIC) {
		 AIC->GetBlackboardComponent()->SetValueAsObject(TargetActorKey, NewTarget);
	}
}

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