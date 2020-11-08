// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "SUGameModeBase.generated.h"

class UEnvQuery;
class UEnvQueryInstanceBlueprintWrapper;
class UCurveFloat;
class ASUPickUpBase;

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASUGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TSubclassOf<AActor> MinionClass;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float SpawnTimerInterval;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UEnvQuery* SpawnBotQuery;

	UPROPERTY(EditDefaultsOnly, Category = "PickUps")
	UEnvQuery* SpawnPickUpsQuery;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	bool ShouldSpawn;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UCurveFloat* DifficultyCurve;

	UPROPERTY(EditDefaultsOnly, Category = "PickUps")
	TMap<TSubclassOf<ASUPickUpBase>, int> PickUpClasses;

	FTimerHandle TimerHandle_SpawnBots;

	UPROPERTY(EditDefaultsOnly, Category = "Player")
	int ValuePerKill;

	UFUNCTION()
	void SpawnBotTimerElapsed();

	UFUNCTION()
	void OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	UFUNCTION()
	void OnQueryCompletedPickUps(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	// We pass in controller because we want to respawn using the controller, not the ASU Character. The Character is as object possessed by the controller.
	UFUNCTION()
	void RespawnPlayerElapsed(AController* Controller);


public:

	ASUGameModeBase();

	virtual void StartPlay() override;

	virtual void OnActorKilled(AActor* VictimActor, AActor* KillerActor);

	UFUNCTION(exec)
	void KillAll();

};
