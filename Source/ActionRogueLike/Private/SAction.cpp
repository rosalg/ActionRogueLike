// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction.h"
#include "../ActionRogueLike.h"
#include "SUActionComponent.h"
#include "Net/UnrealNetwork.h"
#include "..\Public\SAction.h"


void USAction::Initialize(USUActionComponent* NewActionComp)
{
	Cost = 0;
	ActionComp = NewActionComp;
}

/* Have to put implementation because we declared these functions as blueprintNative*/
void USAction::StartAction_Implementation(AActor* Instigator)
{
	// UE_LOG(LogTemp, Log, TEXT("Running: %s"), *GetNameSafe(this));

	LogOnScreen(this, FString::Printf(TEXT("Started: %s"), *ActionName.ToString()), FColor::Green);

	USUActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.AppendTags(GrantsTags);

	RepData.bIsRunning = true;
	RepData.Instigator = Instigator;

	if (GetOwningComponent()->GetOwnerRole() == ROLE_Authority) {
		TimeStarted = GetWorld()->TimeSeconds;
	}

	GetOwningComponent()->OnActionStarted.Broadcast(GetOwningComponent(), this);
}


void USAction::StopAction_Implementation(AActor* Instigator)
{
	// UE_LOG(LogTemp, Log, TEXT("Stopped: %s"), *GetNameSafe(this));

	LogOnScreen(this, FString::Printf(TEXT("Started: %s"), *ActionName.ToString()), FColor::White);

	USUActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.RemoveTags(GrantsTags);

	RepData.bIsRunning = false;
	RepData.Instigator = Instigator;

	GetOwningComponent()->OnActionStopped.Broadcast(GetOwningComponent(), this);
}


void USAction::OnRep_RepData() {
	if (RepData.bIsRunning) {
		StartAction(RepData.Instigator);
	}
	else 
	{
		StopAction(RepData.Instigator);
	}
}




bool USAction::CanStart_Implementation(AActor* Instigator)
{
	if (RepData.bIsRunning) return false;

	USUActionComponent* Comp = GetOwningComponent();


	if (Comp->ActiveGameplayTags.HasAny(BlockedTags)) {
		return false;
	}
	return true;
}


UWorld* USAction::GetWorld() const
{
	AActor* Actor = Cast<AActor>(GetOuter());
	if (Actor) {
		return Actor->GetWorld();
	}
	return nullptr;
}


USUActionComponent* USAction::GetOwningComponent() const {
	
	return ActionComp;
	// return Cast<USUActionComponent>(GetOuter());
}


bool USAction::IsRunning() const {
	return RepData.bIsRunning;
}


void USAction::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USAction, RepData);
	DOREPLIFETIME(USAction, ActionComp);
	DOREPLIFETIME(USAction, TimeStarted);
}

