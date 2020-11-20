// Fill out your copyright notice in the Description page of Project Settings.


#include "SUActionComponent.h"
#include "SAction.h"
#include "../ActionRogueLike.h"
#include "..\Public\SUActionComponent.h"
#include "Net/UnrealNetwork.h"
#include "Engine/ActorChannel.h"

USUActionComponent::USUActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);
}

void USUActionComponent::ServerStopAction_Implementation(AActor* Instigator, FName ActionName)
{
	StopActionByName(Instigator, ActionName);
}

void USUActionComponent::ServerStartAction_Implementation(AActor* Instigator, FName ActionName)
{
	StartActionByName(Instigator, ActionName);
}

// Being play gets called on everything (Server AND clients) but we want it to be server-only.
void USUActionComponent::BeginPlay()
{
	Super::BeginPlay();
	// This check ensures it is only ran on the servers (since only the server has authority). 
	if (GetOwner()->HasAuthority()) {
		for (TSubclassOf<USAction> ActionClass : DefaultActions) {
			AddAction(GetOwner(), ActionClass);
		}
	}
	
}


void USUActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// FString DebugMsg = GetNameSafe(GetOwner()) + " : " + ActiveGameplayTags.ToStringSimple();
	// GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::White, DebugMsg);

	for (USAction* Action : Actions) {
		FColor TextColor = Action->IsRunning() ? FColor::Blue : FColor::White;
		FString ActionMsg = FString::Printf(TEXT("[%s] Action: %s"), *GetNameSafe(GetOwner()), *GetNameSafe(Action));
		LogOnScreen(this, ActionMsg, TextColor, 0.0f);
	}
}

void USUActionComponent::AddAction(AActor* Instigator, TSubclassOf<USAction> ActionClass) {
	if (!ensure(ActionClass)) {
		return;
	}

	if (!GetOwner()->HasAuthority()) {
		UE_LOG(LogTemp, Warning, TEXT("Client attemping to AddAction. [Class: %s]"), *GetNameSafe(ActionClass));
		return;
	}

	USAction* NewAction = NewObject<USAction>(GetOwner(), ActionClass);
	if (ensure(NewAction)) {
		NewAction->Initialize(this);
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

			// Is client?
			if (!GetOwner()->HasAuthority())
				ServerStartAction(Instigator, ActionName);

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

				if (!GetOwner()->HasAuthority())
					ServerStopAction(Instigator, ActionName);

				Action->StopAction(Instigator);
				return true;
			}
		}
	}
	return false;
}

void USUActionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USUActionComponent, Actions);
}

bool USUActionComponent::ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) {
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	// Tells unreal we have an array of subobjects and we want to replicate them
	for (USAction* Action : Actions) {
		if (Action) {
			WroteSomething |= Channel->ReplicateSubobject(Action, *Bunch, *RepFlags);
		}
	}

	return WroteSomething;
}
