// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "SAction.generated.h"

class UWorld;

USTRUCT()
struct FActionRepData {
	GENERATED_BODY()

public:
	UPROPERTY()
	bool bIsRunning;

	UPROPERTY()
	AActor* Instigator;
};


/**
 * 
 */
UCLASS(Blueprintable)
class ACTIONROGUELIKE_API USAction : public UObject
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	UTexture2D* Icon;

	// Replicated because our server needs to know all clients action comps
	UPROPERTY(Replicated)
	USUActionComponent* ActionComp;

	UFUNCTION(BlueprintCallable, Category = "Action")
	USUActionComponent* GetOwningComponent() const;

	/* Tags added to owning actor when activated, removed when action stops*/
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer GrantsTags;

	/* Action can only run if owning actor has none of these Tags applied */
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer BlockedTags;

	UPROPERTY(ReplicatedUsing = "OnRep_RepData")
	FActionRepData RepData;
	//bool bIsRunning;

	UFUNCTION()
	void OnRep_RepData();
	
	UPROPERTY(Replicated)
	float TimeStarted;


public:

	void Initialize(USUActionComponent* NewActionComp);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Action")
	float Cost;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Action")
	TMap<FGameplayTag, TSubclassOf<USAction>> UnlockedTagsOnPurchase;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Action")
	FGameplayTagContainer Prerequisites;

	UPROPERTY(EditDefaultsOnly, Category = "Action")
	bool bAutoStart;

	UFUNCTION(BlueprintCallable, Category = "Action")
	bool IsRunning() const;

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	bool CanStart(AActor* Instigator);

	/* Action nickname to start/stop without a reference to the object */
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	FName ActionName; // This is hashed! Makes it super quick >:)

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StartAction(AActor* Instigator);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Action")
	void StopAction(AActor* Instigator);



	UWorld* GetWorld() const override;

	// When deriving from UObjects, you need to override this to make it networking compatible and say that it is supported for networking.
	bool IsSupportedForNetworking() const override { return true; }

};
