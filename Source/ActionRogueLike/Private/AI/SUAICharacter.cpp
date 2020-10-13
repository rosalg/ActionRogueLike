// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SUAICharacter.h"
#include "Perception/PawnSensingComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "SUAttributeComponent.h"

// Sets default values
ASUAICharacter::ASUAICharacter()
{
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");

	AttributeComp = CreateDefaultSubobject<USUAttributeComponent>("AttributeComp");
}

void ASUAICharacter::PostInitializeComponents() {
	Super::PostInitializeComponents();

	PawnSensingComp->OnSeePawn.AddDynamic(this, &ASUAICharacter::OnPawnSeen);
}

void ASUAICharacter::OnPawnSeen(APawn* Pawn) {
	AAIController* AIC = Cast<AAIController>(GetController());
	if (AIC) {
		UBlackboardComponent* BBComp = AIC->GetBlackboardComponent();

		BBComp->SetValueAsObject("TargetActor", Pawn);

	}


}