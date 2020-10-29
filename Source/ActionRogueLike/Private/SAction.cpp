// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction.h"
#include "SUActionComponent.h"



/* Have to put implementation because we declared these functions as blueprintNative*/
void USAction::StartAction_Implementation(AActor* Instigator)
{
	// UE_LOG(LogTemp, Log, TEXT("Running: %s"), *GetNameSafe(this));

	USUActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.AppendTags(GrantsTags);

	bIsRunning = true;
}

void USAction::StopAction_Implementation(AActor* Instigator)
{
	// UE_LOG(LogTemp, Log, TEXT("Stopped: %s"), *GetNameSafe(this));
	USUActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.RemoveTags(GrantsTags);

	bIsRunning = false;
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
	UActorComponent* Comp = Cast<UActorComponent>(GetOuter());
	if (Comp) {
		return Comp->GetWorld();
	}
	return nullptr;
}

USUActionComponent* USAction::GetOwningComponent() const {
	return Cast<USUActionComponent>(GetOuter());
}

bool USAction::IsRunning() const {
	return bIsRunning;
}