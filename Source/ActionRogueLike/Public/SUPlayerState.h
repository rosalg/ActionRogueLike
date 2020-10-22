// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SUPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASUPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable)
	int GetNumCredits();

	UFUNCTION(BlueprintCallable)
	bool UpdateCredits(AActor* CreditInstigator, int Delta);

	ASUPlayerState();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int NumCredits;

};
