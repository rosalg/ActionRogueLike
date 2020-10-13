
#include "AI/SUBTService_CheckLowHealth.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "SUAttributeComponent.h"
#include "AI/SUAICharacter.h"

void USUBTService_CheckLowHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) {
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// Goal of this code is to check distance between AI pawn and target actor
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (ensure(BlackboardComp)) {
		AAIController* MyController = OwnerComp.GetAIOwner();
		if (ensure(MyController)) {
			APawn* AIPawn = MyController->GetPawn();
			if (ensure(AIPawn)) {
				USUAttributeComponent* AttributeComp = Cast<USUAttributeComponent>(AIPawn->GetComponentByClass(USUAttributeComponent::StaticClass()));
				if (AttributeComp) {
					BlackboardComp->SetValueAsBool(LowHealthKey.SelectedKeyName, AttributeComp->GetCurrentHealth() < HealthThreshold);
				}
			}
		}
		
	}

}

