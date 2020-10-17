// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SUAICharacter.generated.h"

class UPawnSensingComponent;
class USUAttributeComponent;
class UUserWidget;
class USUWorldUserWidget;

UCLASS()
class ACTIONROGUELIKE_API ASUAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASUAICharacter();

protected:

	USUWorldUserWidget* ActiveHealthBar;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	FName TimeToHitParamName;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComp;

	UPROPERTY(VisibleAnywhere)
	USUAttributeComponent* AttributeComp;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> HealthBarWidgetClass;



	virtual void PostInitializeComponents() override;



	UFUNCTION()
	void SetTargetActor(AActor* NewTarget);

	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USUAttributeComponent* OwningComp, float NewHealth, float Delta);
};
