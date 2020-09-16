// Fill out your copyright notice in the Description page of Project Settings.


#include "SUCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera\CameraComponent.h"

// Sets default values
ASUCharacter::ASUCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);
}

// Called when the game starts or when spawned
void ASUCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASUCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASUCharacter::MoveForward(float Value) {
	AddMovementInput(GetActorForwardVector(), Value);
}

// Called to bind functionality to input
void ASUCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASUCharacter::MoveForward);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
}

