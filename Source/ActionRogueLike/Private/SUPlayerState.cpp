// Fill out your copyright notice in the Description page of Project Settings.


#include "SUPlayerState.h"

ASUPlayerState::ASUPlayerState() {
	NumCredits = 0;
}

/*
* Used to add credits to the instigating player.
*/
bool ASUPlayerState::UpdateCredits(AActor* CreditInstigator, int Delta)
{
	NumCredits += Delta;
	UE_LOG(LogTemp, Log, TEXT("PlayerCharacter: %s now has %d credits"), *GetNameSafe(CreditInstigator), NumCredits);
	return true;
}

int ASUPlayerState::GetNumCredits() {
	return NumCredits;
}