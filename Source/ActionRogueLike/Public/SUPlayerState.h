// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "GameplayTagContainer.h"
#include "SUPlayerState.generated.h"

class USUSaveGame;
class USAction;

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

	UFUNCTION(BlueprintNativeEvent)
	void SavePlayerState(USUSaveGame* SaveObject);

	UFUNCTION(BlueprintNativeEvent)
	void LoadPlayerState(USUSaveGame* SaveObject);

	UFUNCTION(BlueprintCallable)
	void AddTagsToUnlocked(TMap<FGameplayTag, TSubclassOf<USAction>> ToAdd);

	UFUNCTION(BlueprintCallable)
	FGameplayTagContainer GetUnlockedTags();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
	TMap<FGameplayTag, TSubclassOf<USAction>> UnlockedAbilities;

	ASUPlayerState();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated)
	int NumCredits;

};
