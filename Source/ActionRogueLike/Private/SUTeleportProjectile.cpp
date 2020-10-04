// Fill out your copyright notice in the Description page of Project Settings.


#include "SUTeleportProjectile.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SUCharacter.h"
#include "GameFramework/ProjectileMovementComponent.h"


ASUTeleportProjectile::ASUTeleportProjectile() {
	PrimaryActorTick.bCanEverTick = true;

	SphereComp->OnComponentHit.AddDynamic(this, &ASUTeleportProjectile::OnCompHit);
}

void ASUTeleportProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void ASUTeleportProjectile::Teleport() {
	GetInstigator()->SetActorLocation(GetActorLocation());
	Destroy();
}

void ASUTeleportProjectile::Detonate() {
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DetonationEffect, GetActorLocation());
}

void ASUTeleportProjectile::OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) {
	//STOP TIMER FROM RUNNING ANY FURTHER
	ASUCharacter* PC = (ASUCharacter*)GetInstigator();
	FTimerHandle* TempHandle = PC->GetTeleportTimerHandle();
	GetWorldTimerManager().ClearTimer(*TempHandle);
	//MovementComp->
	Detonate();
	GetWorldTimerManager().SetTimer(Delay, this, &ASUTeleportProjectile::Teleport, 0.2f);
}
/*
// Called every frame
void ASUTeleportProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}*/