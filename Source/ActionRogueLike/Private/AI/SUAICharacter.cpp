// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SUAICharacter.h"
#include "Perception/PawnSensingComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "SUAttributeComponent.h"
#include "BrainComponent.h"

// Sets default values
ASUAICharacter::ASUAICharacter()
{
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");

	AttributeComp = CreateDefaultSubobject<USUAttributeComponent>("AttributeComp");
}

void ASUAICharacter::PostInitializeComponents() {
	Super::PostInitializeComponents();

	PawnSensingComp->OnSeePawn.AddDynamic(this, &ASUAICharacter::OnPawnSeen);
	AttributeComp->OnHealthChanged.AddDynamic(this, &ASUAICharacter::OnHealthChanged);
}

void ASUAICharacter::OnPawnSeen(APawn* Pawn) {
	AAIController* AIC = Cast<AAIController>(GetController());
	if (AIC) {
		UBlackboardComponent* BBComp = AIC->GetBlackboardComponent();

		BBComp->SetValueAsObject("TargetActor", Pawn);

	}


}

void ASUAICharacter::OnHealthChanged(AActor* InstigatorActor, USUAttributeComponent* OwningComp, float NewHealth, float Delta) {
	if (Delta < 0.0f) {
		if (NewHealth <= 0.0f) {
			// Stop BT
			AAIController* AIC = Cast<AAIController>(GetController());
			if (AIC) {
				AIC->GetBrainComponent()->StopLogic("Killed");
			}

			// Ragdoll
			GetMesh()->SetAllBodiesSimulatePhysics(true);
			GetMesh()->SetCollisionProfileName("Ragdoll");

			// set lifespan
			SetLifeSpan(10.0f);
		}
	}
}