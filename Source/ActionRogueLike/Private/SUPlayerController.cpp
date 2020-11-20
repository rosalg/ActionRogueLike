// Fill out your copyright notice in the Description page of Project Settings.


#include "SUPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "SUPlayerState.h"
#include "SUCharacter.h"
#include "SUActionComponent.h"

void ASUPlayerController::TogglePauseMenu()
{

	if (PauseMenuInstance && PauseMenuInstance->IsInViewport()) {
		PauseMenuInstance->RemoveFromParent();
		PauseMenuInstance = nullptr;

		bShowMouseCursor = false;
		SetInputMode(FInputModeGameOnly());
		return;
	}

	PauseMenuInstance = CreateWidget<UUserWidget>(this, PauseMenuClass);
	if (PauseMenuInstance) {
		PauseMenuInstance->AddToViewport(100);

		bShowMouseCursor = true;
		SetInputMode(FInputModeUIOnly());


	}
}

void ASUPlayerController::TogglePowerUpMenu()
{

	if (PowerUpMenuInstance && PowerUpMenuInstance->IsInViewport()) {
		PowerUpMenuInstance->RemoveFromParent();
		PowerUpMenuInstance = nullptr;

		bShowMouseCursor = false;
		SetInputMode(FInputModeGameOnly());
		return;
	}

	PowerUpMenuInstance = CreateWidget<UUserWidget>(this, PowerUpMenuClass);
	if (PowerUpMenuInstance) {
		PowerUpMenuInstance->AddToViewport(100);

		bShowMouseCursor = true;


	}
}

void ASUPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("PauseMenu", IE_Pressed, this, &ASUPlayerController::TogglePauseMenu);
	InputComponent->BindAction("PowerUpMenu", IE_Pressed, this, &ASUPlayerController::TogglePowerUpMenu);
}

void ASUPlayerController::SetPawn(APawn* InPawn) {
	Super::SetPawn(InPawn);
	if (InPawn) {
		ASUPlayerState* PS = Cast<ASUPlayerState>(InPawn->GetPlayerState());

		if (PS) {
			USUActionComponent* ActionComp = Cast<USUActionComponent>(InPawn->GetComponentByClass(USUActionComponent::StaticClass()));

			if (ActionComp) {
				for (auto curr: PS->UnlockedAbilities)
				ActionComp->AddAction(InPawn, curr.Value);
			}
			

		}
	}
	OnPawnChanged.Broadcast(InPawn);
}
