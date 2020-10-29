// Fill out your copyright notice in the Description page of Project Settings.


#include "SUCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera\CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"
#include "SUInteractionComponent.h"
#include "SUTeleportProjectile.h"
#include "SUAttributeComponent.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"
#include "SUActionComponent.h"

// Sets default values
ASUCharacter::ASUCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);

	InteractionComp = CreateDefaultSubobject<USUInteractionComponent>("InteractionComp");
	
	AttributeComp = CreateDefaultSubobject<USUAttributeComponent>("AttributeComp");

	ActionComp = CreateDefaultSubobject<USUActionComponent>("ActionComp");

	GetCharacterMovement()->bOrientRotationToMovement = true;

	bUseControllerRotationYaw = false;

	AttributeComp->OnRageChange.AddDynamic(this, &ASUCharacter::OnRageChange);
	AttributeComp->OnHealthChanged.AddDynamic(this, &ASUCharacter::OnHealthChanged);
}

/*void ASUCharacter::PostInitializeComponents() {
	Super::PostInitializeComponents();
}*/

// Called when the game starts or when spawned
void ASUCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASUCharacter::OnRageChange(AActor* InstigatorActor, USUAttributeComponent* OwningComp, float NewRage, float Delta) {
	UE_LOG(LogTemp, Log, TEXT("Rage change with value %f"), Delta);
}

// Called every frame
void ASUCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector ASUCharacter::GetPawnViewLocation() const {
	return CameraComp->GetComponentLocation();
}

/*
 * This function is listening to the event OnHealthChanged this is a part of our AttributeComponent. 
 * 
 * When invoked (check damaging or healing script (i.e. ASUMagicProjectile) to see broadcast), it will update the TimeToHit parameter on
 * its static mesh component.
 * 
 * PARAMETERS
 *		AActor* InstigatorActor - Actor that caused the healing or damage
 *		USUAttributeComponent* OwningComp - Component that this event belongs to
 *		float NewHealth - Health post change
 *		float Delta - The amount health changed
 * RETURN
 *		None
*/
void ASUCharacter::OnHealthChanged(AActor* InstigatorActor, USUAttributeComponent* OwningComp, float NewHealth, float Delta, float NewRage) {
	Cast<ACharacter>(this)->GetMesh()->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);
	if (NewHealth <= 0.0f && Delta < 0.0f) {
		APlayerController* PC = Cast<APlayerController>(GetController());
		DisableInput(PC);
	}
}

void ASUCharacter::MoveForward(float Value) {

	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	AddMovementInput(ControlRot.Vector(), Value);
}

void ASUCharacter::MoveRight(float Value) {
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);

	AddMovementInput(RightVector, Value);
}

void ASUCharacter::SprintStart()
{
	ActionComp->StartActionByName(this, "Sprint");
}

void ASUCharacter::SprintStop()
{
	ActionComp->StopActionByName(this, "Sprint");
}

void ASUCharacter::PrimaryAttack() {
	ActionComp->StartActionByName(this, "PrimaryAttack");
}

void ASUCharacter::BlackHoleAttack()
{
	ActionComp->StartActionByName(this, "BlackHoleAttack");
}

void ASUCharacter::Teleport()
{
	ActionComp->StartActionByName(this, "Teleport");
}

void ASUCharacter::FullHeal()
{
	AttributeComp->ApplyHealthChange(this, AttributeComp->GetMaxHealth());
}

void ASUCharacter::PrimaryInteract() {
	InteractionComp->PrimaryInteract();
}

// Called to bind functionality to input
void ASUCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASUCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASUCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &ASUCharacter::PrimaryAttack);
	PlayerInputComponent->BindAction("UltimateAttack", IE_Pressed, this, &ASUCharacter::BlackHoleAttack);
	PlayerInputComponent->BindAction("Teleport", IE_Pressed, this, &ASUCharacter::Teleport);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &ASUCharacter::PrimaryInteract);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ASUCharacter::SprintStart);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ASUCharacter::SprintStop);
}

