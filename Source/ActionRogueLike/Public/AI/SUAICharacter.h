// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SUAICharacter.generated.h"

class UPawnSensingComponent;
class USUAttributeComponent;


UCLASS()
class ACTIONROGUELIKE_API ASUAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASUAICharacter();

protected:

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComp;

	UPROPERTY(VisibleAnywhere)
	USUAttributeComponent* AttributeComp;

	virtual void PostInitializeComponents() override;

	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);

};
