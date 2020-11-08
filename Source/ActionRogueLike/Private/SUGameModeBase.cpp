// Fill out your copyright notice in the Description page of Project Settings.


#include "SUGameModeBase.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "SUAttributeComponent.h"
#include "AI/SUAICharacter.h"
#include "EngineUtils.h"
#include <SUTeleportProjectile.h>
#include <SUCharacter.h>
#include "GameFramework/PlayerState.h"
#include "SUPlayerState.h"
#include "SUPickUpBase.h"
#include "Kismet/KismetArrayLibrary.h"



static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("su.SpawnBots"), true, TEXT("Enable spawning of bots via timer."), ECVF_Cheat);

ASUGameModeBase::ASUGameModeBase() {
	SpawnTimerInterval = 2.0f;
	ValuePerKill = 1;
	ShouldSpawn = false;
}

void ASUGameModeBase::StartPlay() {
	Super::StartPlay();

	
	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnPickUpsQuery, this, EEnvQueryRunMode::AllMatching, nullptr);
	if (ensure(QueryInstance)) {
		UE_LOG(LogTemp, Log, TEXT("Succeeded query, now entering OnQueryCompletedPickups"));
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASUGameModeBase::OnQueryCompletedPickUps);
	}
	

	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &ASUGameModeBase::SpawnBotTimerElapsed, SpawnTimerInterval, true);
}

void ASUGameModeBase::KillAll()
{
	for (TActorIterator<ASUAICharacter> It(GetWorld()); It; ++It) {
		ASUAICharacter* Bot = *It;

		USUAttributeComponent* AttributeComp = USUAttributeComponent::GetAttributes(Bot);
		if (AttributeComp) {
			AttributeComp->Kill(this);
		}
	}
}

void ASUGameModeBase::SpawnBotTimerElapsed() {
	if (!CVarSpawnBots.GetValueOnGameThread()) {
		// UE_LOG(LogTemp, Warning, TEXT("Bot spawning disabled."));
		return;
	}
	int32 NumAliveBots = 0;
	for (TActorIterator<ASUAICharacter> It(GetWorld()); It; ++It) {
		ASUAICharacter* Bot = *It;

		USUAttributeComponent* AttributeComp = USUAttributeComponent::GetAttributes(Bot);
		if (AttributeComp) {
			NumAliveBots++;
		}
	}

	float MaxBotCount = 10.0f;

	if (DifficultyCurve) {
		MaxBotCount = DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);
	}

	if (NumAliveBots >= MaxBotCount) {
		return;
	}

	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);
	if (ensure(QueryInstance))
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASUGameModeBase::OnQueryCompleted);
}

void ASUGameModeBase::OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus) {
	if (QueryStatus != EEnvQueryStatus::Success || !ShouldSpawn) {
		return;
	}
	
	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();

	if (Locations.Num() > 0) {
		GetWorld()->SpawnActor<AActor>(MinionClass, Locations[0], FRotator::ZeroRotator);
	}
}

void ASUGameModeBase::OnQueryCompletedPickUps(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success) {
		return;
	}

	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();
	TArray<int> LocationIndices;
	for (int32 i = 0; i < Locations.Num() - 1; i++) {
		LocationIndices.Push(i);
	}

	for (int i = 0; i < LocationIndices.Num(); i++) {
		int32 Index = FMath::RandRange(i, LocationIndices.Num() - 1);
		if (i != Index)
		{
			LocationIndices.Swap(i, Index);
		}
	}

	int numTotalsSpawns = 0;
	for (const TPair<TSubclassOf<ASUPickUpBase>, int>& pair : PickUpClasses) {
		int CurrTotal = numTotalsSpawns;
		for (int i = numTotalsSpawns; i < CurrTotal + pair.Value; i++) {
			if (Locations.Num() > i) {
				GetWorld()->SpawnActor<AActor>(pair.Key, Locations[LocationIndices[i]], FRotator::ZeroRotator);
				numTotalsSpawns++;
			}
		}
	}
	
}

void ASUGameModeBase::RespawnPlayerElapsed(AController* Controller) {
	if (ensure(Controller)) {

		Controller->UnPossess();
		RestartPlayer(Controller);
	}
}

void ASUGameModeBase::OnActorKilled(AActor* VictimActor, AActor* KillerActor) {
	ASUCharacter* Player = Cast<ASUCharacter>(VictimActor);
	ASUAICharacter* AIChar = Cast<ASUAICharacter>(VictimActor);
	if (Player) {
		// Important this is local, to make it specific to each actor (create a new timer object)
		FTimerHandle TimerHandle_RespawnDelay;
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "RespawnPlayerElapsed", Player->GetController());

		float RespawnDelay = 2.0f;
		GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay, Delegate, RespawnDelay, false);
	}
	else if (AIChar) {
		Player = Cast<ASUCharacter>(KillerActor);
		if (Player) {
			ASUPlayerState* State = Cast<ASUPlayerState>(Player->GetPlayerState());
			State->UpdateCredits(Player, ValuePerKill);
		}
	}
}