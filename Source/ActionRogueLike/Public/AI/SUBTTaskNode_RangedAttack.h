// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "SUBTTaskNode_RangedAttack.generated.h"

class SUBaseProjectile;

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API USUBTTaskNode_RangedAttack : public UBTTaskNode
{
	GENERATED_BODY()

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;


protected:	

	UPROPERTY(EditAnywhere, Category = "AI")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "AI")
	float MaxBulletSpread;

public:

	USUBTTaskNode_RangedAttack();
};
