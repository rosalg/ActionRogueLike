// Fill out your copyright notice in the Description page of Project Settings.


#include "SUActionComponent.h"
#include "SAction.h"

USUActionComponent::USUActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void USUActionComponent::BeginPlay()
{
	Super::BeginPlay();

	for (TSubclassOf<USAction> ActionClass : DefaultActions) {
		AddAction(ActionClass);
	}
}


void USUActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void USUActionComponent::AddAction(TSubclassOf<USAction> ActionClass) {
	if (!ensure(ActionClass)) {
		return;
	}

	USAction* NewAction = NewObject<USAction>(this, ActionClass);
	if (ensure(NewAction)) {
		Actions.Add(NewAction);
	}
}

bool USUActionComponent::StartActionByName(AActor* Instigator, FName ActionName)
{
	for (USAction* Action : Actions) {
		if (Action && Action->ActionName == ActionName) {
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
			Action->StopAction(Instigator);
			return true;
		}
	}
	return false;
}
