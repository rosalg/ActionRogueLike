// Fill out your copyright notice in the Description page of Project Settings.


#include "ASUMagicProjectile.h"
#include "Components/SphereComponent.h"
#include "SUAttributeComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Particles/ParticleSystem.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "SUGameplayFunctionLibrary.h"

// Sets default values
AASUMagicProjectile::AASUMagicProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AASUMagicProjectile::OnActorOverlap);
	MovementComp->InitialSpeed = 3000.f;
	Damage = 20.f;
}

void AASUMagicProjectile::OnActorOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	AActor* inst = GetInstigator();
	if (OtherActor && OtherActor != inst) {
		/*USUAttributeComponent* AttributeComp = Cast<USUAttributeComponent>(OtherActor->GetComponentByClass(USUAttributeComponent::StaticClass()));
		if (AttributeComp) {
			AttributeComp->ApplyHealthChange(GetInstigator(), Damage);
		}
		
		*/
		if (USUGameplayFunctionLibrary::ApplyDirectionalDamage(inst, OtherActor, Damage, SweepResult)) {
			UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation());
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticle, GetActorLocation(), GetActorRotation());
			Destroy();
		}
	}
}


// Called when the game starts or when spawned
void AASUMagicProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AASUMagicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

