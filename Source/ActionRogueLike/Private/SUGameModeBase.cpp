// Fill out your copyright notice in the Description page of Project Settings.


#include "SUGameModeBase.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "SUAttributeComponent.h"
#include "AI/SUAICharacter.h"
#include "EngineUtils.h"


ASUGameModeBase::ASUGameModeBase() {
	SpawnTimerInterval = 2.0f;
}

void ASUGameModeBase::StartPlay() {
	Super::StartPlay();


	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &ASUGameModeBase::SpawnBotTimerElapsed, SpawnTimerInterval, true);
}

void ASUGameModeBase::SpawnBotTimerElapsed() {
	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);
	if (ensure(QueryInstance))
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASUGameModeBase::OnQueryCompleted);
}

void ASUGameModeBase::OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus) {
	if (QueryStatus != EEnvQueryStatus::Success) {
		UE_LOG(LogTemp, Warning, TEXT("Spawn bot EQS Query Failed!"));
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Querying"));
	int32 NumAliveBots = 0;
	for (TActorIterator<ASUAICharacter> It(GetWorld()); It; ++It) {
		ASUAICharacter* Bot = *It;

		USUAttributeComponent* AttributeComp = Cast<USUAttributeComponent>(Bot->GetComponentByClass(USUAttributeComponent::StaticClass()));
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

	

	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();

	if (Locations.Num() > 0) {
		GetWorld()->SpawnActor<AActor>(MinionClass, Locations[0], FRotator::ZeroRotator);
	}
}