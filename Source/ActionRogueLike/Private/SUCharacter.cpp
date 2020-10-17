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


const int PRIMARY_PROJ_INDEX = 0;
const int ULTIMATE_PROJ_INDEX = 1;
const int TELEPORT_PROJ_INDEX = 2;

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
	AttributeComp->OnHealthChanged.AddDynamic(this, &ASUCharacter::OnHealthChanged);

	GetCharacterMovement()->bOrientRotationToMovement = true;

	bUseControllerRotationYaw = false;

	
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
void ASUCharacter::OnHealthChanged(AActor* InstigatorActor, USUAttributeComponent* OwningComp, float NewHealth, float Delta) {
	Cast<ACharacter>(this)->GetMesh()->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);
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

void ASUCharacter::PrimaryAttack(int ProjectileNum) {
	PlayAnimMontage(AttackAnim);

	FTimerDelegate TimerDel;

	TimerDel.BindUFunction(this, FName("PrimaryAttack_TimeElapsed"), ProjectileNum);

	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, TimerDel, 0.2f, false);

}

void ASUCharacter::PrimaryAttack_TimeElapsed(int ProjectileNum) {
	FVector handLocation = GetMesh()->GetSocketLocation("Muzzle_01");
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleFlash, handLocation, GetActorRotation());
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_PhysicsBody);
	// Set up raycast, lining up shot with camera and hand rather than directly from where our character is facing.
	FVector start = CameraComp->GetComponentLocation();
	FVector end = start + (CameraComp->GetForwardVector() * 100000);
	FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType(Hit, start, end, ObjectQueryParams);
	FRotator bulletDirection;
	if (Hit.bBlockingHit) {
		bulletDirection = UKismetMathLibrary::FindLookAtRotation(handLocation, Hit.ImpactPoint);
	}
	else {
		bulletDirection = UKismetMathLibrary::FindLookAtRotation(handLocation, end);
	}
	FTransform SpawnTM = FTransform(bulletDirection, handLocation);
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = this;
	// Determine which projectile to spawn based on input parameter
	if (ProjectileNum == PRIMARY_PROJ_INDEX) {
		GetWorld()->SpawnActor<AActor>(PrimaryProjectileClass, SpawnTM, SpawnParams);
	}
	else if (ProjectileNum == ULTIMATE_PROJ_INDEX) {
		GetWorld()->SpawnActor<AActor>(UltimateProjectileClass, SpawnTM, SpawnParams);
	}
	else if (ProjectileNum == TELEPORT_PROJ_INDEX) {
		ASUTeleportProjectile* tp_proj = GetWorld()->SpawnActor<ASUTeleportProjectile>(TeleportProjectileClass, SpawnTM, SpawnParams);
		FTimerDelegate TeleportDurationExpiredDelegate;
		TeleportDurationExpiredDelegate.BindUFunction(this, FName("TeleportDurationExpired"), tp_proj);
		GetWorldTimerManager().SetTimer(TimerHandle_Teleport, TeleportDurationExpiredDelegate, 0.2f, false);
	}
 	
}


void ASUCharacter::TeleportDurationExpired(ASUTeleportProjectile* Bullet) {
	Bullet->Detonate();
	FTimerHandle DestroyTimerHandle;
	FTimerDelegate DestroyDelegate;
	DestroyDelegate.BindUFunction(this, FName("AnimationDurationExpired"), Bullet);
	GetWorldTimerManager().SetTimer(DestroyTimerHandle, DestroyDelegate, 0.2f, false);
}


void ASUCharacter::AnimationDurationExpired(ASUTeleportProjectile* Bullet) {
	Bullet->Teleport();
}


FTimerHandle* ASUCharacter::GetTeleportTimerHandle() {
	return &TimerHandle_Teleport;
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

	PlayerInputComponent->BindAction<Attack>("PrimaryAttack", IE_Pressed, this, &ASUCharacter::PrimaryAttack, PRIMARY_PROJ_INDEX);
	PlayerInputComponent->BindAction<Attack>("UltimateAttack", IE_Pressed, this, &ASUCharacter::PrimaryAttack, ULTIMATE_PROJ_INDEX);
	PlayerInputComponent->BindAction<Attack>("Teleport", IE_Pressed, this, &ASUCharacter::PrimaryAttack, TELEPORT_PROJ_INDEX);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &ASUCharacter::PrimaryInteract);
}

