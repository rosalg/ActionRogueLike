// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SUBTService.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

/*
* This function is overridden from the BTService class,
* allowing us to implement a service that is used in a AI Logic tree.
* Currently, it takes our blackboard, gets the target actor, gets the AI controller,
* Then, if we are in range of the target actor, update a boolean to say the user of this service
* is ready to perform some action.
* 
* @param OwnerComp - Owning component of tick node.
* @param NodeMemory - Memory
* @param DeltaSeconds - Amount of time passed per tick.
*/
void USUBTService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) {
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// Goal of this code is to check distance between AI pawn and target actor
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (ensure(BlackboardComp)) {
		AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject("TargetActor"));
		if (TargetActor) {
			AAIController* MyController = OwnerComp.GetAIOwner();
			if (ensure(MyController)) {
				APawn* AIPawn = MyController->GetPawn();
				if (ensure(AIPawn)) {
					float DistanceTo = FVector::Distance(TargetActor->GetActorLocation(), AIPawn->GetActorLocation());
					
					bool bWithinRange = DistanceTo < MaxRange;
					bool bHasLOS = false;
					if (bWithinRange) {
						bHasLOS = MyController->LineOfSightTo(TargetActor);
					}
					BlackboardComp->SetValueAsBool(AttackRangeKey.SelectedKeyName, bWithinRange && bHasLOS);
				}
			}
		}
	}

}
