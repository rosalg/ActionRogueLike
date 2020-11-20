// Fill out your copyright notice in the Description page of Project Settings.


#include "SUPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "SUSaveGame.h"
#include "SAction.h"

void ASUPlayerState::AddTagsToUnlocked(TMap<FGameplayTag, TSubclassOf<USAction>> ToAdd)
{
	UnlockedAbilities.Append(ToAdd);
}

FGameplayTagContainer ASUPlayerState::GetUnlockedTags() {
	FGameplayTagContainer output;
	for (auto curr : UnlockedAbilities) {
		output.AddTag(curr.Key);
	}
	return output;
}

ASUPlayerState::ASUPlayerState() {
	NumCredits = 0;
}

/*
* Used to add credits to the instigating player.
*/
bool ASUPlayerState::UpdateCredits(AActor* CreditInstigator, int Delta)
{
	NumCredits += Delta;
	return true;
}

int ASUPlayerState::GetNumCredits() {
	return NumCredits;
}

void ASUPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASUPlayerState, NumCredits);

	// This is just for optimization for bandwidth to minimize how much we need to send over.
	// DOREPLIFETIME_CONDITION(USUAttributeComponent, HealthMax, COND_OwnerOnly);
}


void ASUPlayerState::SavePlayerState_Implementation(USUSaveGame* SaveObject) {
	if (SaveObject) {
		SaveObject->Credits = NumCredits;
	}
}



void ASUPlayerState::LoadPlayerState_Implementation(USUSaveGame* SaveObject) {
	if (SaveObject) {
		NumCredits = SaveObject->Credits;
	}
}
