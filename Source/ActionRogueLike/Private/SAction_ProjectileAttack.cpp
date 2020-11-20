// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction_ProjectileAttack.h"
#include "GameFramework/Character.h"
#include <Kismet\GameplayStatics.h>
#include <Kismet\KismetMathLibrary.h>


USAction_ProjectileAttack::USAction_ProjectileAttack()
{
	HandSocketName = "Muzzle_01";
	AttackAnimDelay = 0.2f;
}

void USAction_ProjectileAttack::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	ACharacter* Character = Cast<ACharacter>(Instigator);
	if (Character) {
		Character->PlayAnimMontage(AttackAnim);

		if (Character->HasAuthority()) {

			FTimerHandle TimerHandle_AttackDelay;
			FTimerDelegate TimerDel;
			TimerDel.BindUFunction(this, FName("AttackDelay_Elapsed"), Instigator);

			GetWorld()->GetTimerManager().SetTimer(TimerHandle_AttackDelay, TimerDel, AttackAnimDelay, false);
		}
	}

}


void USAction_ProjectileAttack::AttackDelay_Elapsed(ACharacter* InstigatorCharacter)
{
	if (ensureAlways(ProjectileClass)) {
		FVector handLocation = InstigatorCharacter->GetMesh()->GetSocketLocation("Muzzle_01");
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleFlash, handLocation, InstigatorCharacter->GetActorRotation());
		
		FCollisionObjectQueryParams ObjectQueryParams;
		ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjectQueryParams.AddObjectTypesToQuery(ECC_PhysicsBody);
		
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(InstigatorCharacter);

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = InstigatorCharacter;

		// Set up raycast, lining up shot with camera and hand rather than directly from where our character is facing.
		FVector start = InstigatorCharacter->GetPawnViewLocation();
		FVector end = start + (InstigatorCharacter->GetControlRotation().Vector() * 100000);
		FHitResult Hit;
		
		FCollisionShape Shape;
		Shape.SetSphere(20.0f);

		
		GetWorld()->LineTraceSingleByObjectType(Hit, start, end, ObjectQueryParams);
		//GetWorld()->SweepSingleByObjectType(Hit, start, end, FQuat::Identity, ObjectQueryParams, Shape, Params);
		FRotator bulletDirection;
		if (Hit.bBlockingHit) {
			bulletDirection = UKismetMathLibrary::FindLookAtRotation(handLocation, Hit.ImpactPoint);
		}
		else {
			bulletDirection = UKismetMathLibrary::FindLookAtRotation(handLocation, end);
		}
		FTransform SpawnTM = FTransform(bulletDirection, handLocation);
		// Determine which projectile to spawn based on input parameter
		GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);
	/*
	else if (ProjectileNum == ULTIMATE_PROJ_INDEX) {
			GetWorld()->SpawnActor<AActor>(UltimateProjectileClass, SpawnTM, SpawnParams);
		}
		else if (ProjectileNum == TELEPORT_PROJ_INDEX) {
			ASUTeleportProjectile* tp_proj = GetWorld()->SpawnActor<ASUTeleportProjectile>(TeleportProjectileClass, SpawnTM, SpawnParams);
			FTimerDelegate TeleportDurationExpiredDelegate;
			TeleportDurationExpiredDelegate.BindUFunction(this, FName("TeleportDurationExpired"), tp_proj);
			GetWorldTimerManager().SetTimer(TimerHandle_Teleport, TeleportDurationExpiredDelegate, 0.2f, false);
		}
		*/
	}
	StopAction(InstigatorCharacter);
}
