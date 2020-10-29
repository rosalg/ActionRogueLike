// Fill out your copyright notice in the Description page of Project Settings.


#include "SUActionComponent.h"
#include "SAction.h"
#include "..\Public\SUActionComponent.h"

USUActionComponent::USUActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void USUActionComponent::BeginPlay()
{
	Super::BeginPlay();

	for (TSubclassOf<USAction> ActionClass : DefaultActions) {
		AddAction(GetOwner(), ActionClass);
	}
}


void USUActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FString DebugMsg = GetNameSafe(GetOwner()) + " : " + ActiveGameplayTags.ToStringSimple();
	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::White, DebugMsg);
}

void USUActionComponent::AddAction(AActor* Instigator, TSubclassOf<USAction> ActionClass) {
	if (!ensure(ActionClass)) {
		return;
	}

	USAction* NewAction = NewObject<USAction>(this, ActionClass);
	if (ensure(NewAction)) {
		Actions.Add(NewAction);

		if (NewAction->bAutoStart && ensure(NewAction->CanStart(Instigator))) {
			NewAction->StartAction(Instigator);
		}
	}
}

void USUActionComponent::RemoveAction(USAction* ActionToRemove)
{
	if (ensure(ActionToRemove && !ActionToRemove->IsRunning())) {
		return;
	}
	Actions.Remove(ActionToRemove);
}

bool USUActionComponent::StartActionByName(AActor* Instigator, FName ActionName)
{
	for (USAction* Action : Actions) {
		if (Action && Action->ActionName == ActionName) {

			if (!Action->CanStart(Instigator)) {
				continue;
			}

			Action->StartAction(Instigator);
			return true;
		}
	}
	return false;
}

bool USUActionComponent::StopActionByName(AActor* Instigator, FName ActionName)
{
	for (USAction* Action : Actions) {
		if (Action && Action->ActionName == ActionName) {

			if (Action->IsRunning()) {
				Action->StopAction(Instigator);
				return true;
			}
		}
	}
	return false;
}
