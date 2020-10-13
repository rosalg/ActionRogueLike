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

UCLASS()
class ACTIONROGUELIKE_API ASUCharacter : public ACharacter
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> PrimaryProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> UltimateProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> TeleportProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackAnim;

	UPROPERTY(EditAnywhere, Category = "Attack")
	UParticleSystem* MuzzleFlash;

	FTimerHandle TimerHandle_PrimaryAttack;
	FTimerHandle TimerHandle_Teleport;



public:
	// Sets default values for this character's properties
	ASUCharacter();

	FTimerHandle* GetTeleportTimerHandle();


protected:

	UPROPERTY(VisibleAnywhere);
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere);
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere);
	USUInteractionComponent* InteractionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	USUAttributeComponent* AttributeComp;

	void MoveForward(float Value);
	void MoveRight(float Value);
	void PrimaryAttack(int ProjectileNum);
	DECLARE_DELEGATE_OneParam(Attack, int);
	UFUNCTION()
	void PrimaryAttack_TimeElapsed(int ProjectileNum);
	void PrimaryInteract();
	UFUNCTION()
	void TeleportDurationExpired(ASUTeleportProjectile* Bullet);
	UFUNCTION()
	void AnimationDurationExpired(ASUTeleportProjectile* Bullet);

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USUAttributeComponent* OwningComp, float NewHealth, float Delta);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
