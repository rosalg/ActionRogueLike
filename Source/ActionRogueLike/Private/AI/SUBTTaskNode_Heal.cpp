// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SUBTTaskNode_Heal.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "SUAttributeComponent.h"

EBTNodeResult::Type USUBTTaskNode_Heal::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController) {
		USUAttributeComponent* AttributeComp = Cast<USUAttributeComponent>(AIController->GetPawn()->GetComponentByClass(USUAttributeComponent::StaticClass()));
		if (AttributeComp) {
			AttributeComp->ApplyHealthChange(AIController->GetPawn(), 50.f);
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}