// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SAction.h"
#include "SUActionEffect.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API USUActionEffect : public USAction
{
	GENERATED_BODY()
	
public:

	void virtual StartAction_Implementation(AActor* Instigator) override;

	void virtual StopAction_Implementation(AActor* Instigator) override;

	UFUNCTION(BlueprintCallable, Category = "Action")
	float GetTimeRemaining() const;

	USUActionEffect();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	float Duration;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	float Period;

	FTimerHandle PeriodHandle;
	FTimerHandle DurationHandle;

	UFUNCTION(BlueprintNativeEvent, Category = "Effects")
	void ExecutePeriodicEffect(AActor* Instigator);
};
