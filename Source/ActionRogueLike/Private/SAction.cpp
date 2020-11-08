// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction.h"
#include "../ActionRogueLike.h"
#include "SUActionComponent.h"
#include "Net/UnrealNetwork.h"


void USAction::Initialize(USUActionComponent* NewActionComp)
{
	ActionComp = NewActionComp;
}

/* Have to put implementation because we declared these functions as blueprintNative*/
void USAction::StartAction_Implementation(AActor* Instigator)
{
	// UE_LOG(LogTemp, Log, TEXT("Running: %s"), *GetNameSafe(this));

	LogOnScreen(this, FString::Printf(TEXT("Started: %s"), *ActionName.ToString()), FColor::Green);

	USUActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.AppendTags(GrantsTags);

	bIsRunning = true;
}


void USAction::StopAction_Implementation(AActor* Instigator)
{
	// UE_LOG(LogTemp, Log, TEXT("Stopped: %s"), *GetNameSafe(this));

	LogOnScreen(this, FString::Printf(TEXT("Started: %s"), *ActionName.ToString()), FColor::White);

	USUActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.RemoveTags(GrantsTags);

	bIsRunning = false;
}


void USAction::OnRep_IsRunning() {
	if (bIsRunning) {
		StartAction(nullptr);
	}
	else {
		StopAction(nullptr);
	}
}




bool USAction::CanStart_Implementation(AActor* Instigator)
{
	if (IsRunning()) return false;

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
	return bIsRunning;
}


void USAction::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USAction, bIsRunning);
	DOREPLIFETIME(USAction, ActionComp);
}

