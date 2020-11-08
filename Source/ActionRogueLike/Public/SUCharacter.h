// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SUCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class USUInteractionComponent;
class ASUTeleportProjectile;
class UAnimMontage;
class USUAttributeComponent;
class UParticleSystem;
class USUActionComponent;

UCLASS()
class ACTIONROGUELIKE_API ASUCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASUCharacter();

	UFUNCTION(exec)
	void FullHeal();

	UFUNCTION(exec)
	void ChangePlayerHealth();

protected:

	UPROPERTY(VisibleAnywhere);
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere);
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere);
	USUInteractionComponent* InteractionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	USUAttributeComponent* AttributeComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USUActionComponent* ActionComp;

	void MoveForward(float Value);
	void MoveRight(float Value);
	/*
	ACTIONS
	*/
	void SprintStart();
	void SprintStop();
	void PrimaryAttack();
	void BlackHoleAttack();
	void Teleport();
	void PrimaryInteract();

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USUAttributeComponent* OwningComp, float NewHealth, float Delta);

	UFUNCTION()
	void OnRageChange(AActor* InstigatorActor, USUAttributeComponent* OwningComp, float NewRage, float Delta);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//virtual void PostInitializeComponents() override;

	virtual FVector GetPawnViewLocation() const override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
