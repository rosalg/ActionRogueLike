// Fill out your copyright notice in the Description page of Project Settings.


#include "SURedBarrel.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Components/StaticMeshComponent.h"



// Sets default values
ASURedBarrel::ASURedBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	RootComponent = MeshComp;

	RadialForceComp = CreateDefaultSubobject<URadialForceComponent>("RadialForceComp");
	RadialForceComp->SetupAttachment(MeshComp);

	MeshComp->SetSimulatePhysics(true);

	RadialForceComp->Radius = 700;
	RadialForceComp->ImpulseStrength = 2000;
	RadialForceComp->bImpulseVelChange = true;
	RadialForceComp->bAutoActivate = false;
	MeshComp->OnComponentHit.AddDynamic(this, &ASURedBarrel::OnCompHit);
	MeshComp->SetCollisionProfileName("PhysicsActor");
}

// Called when the game starts or when spawned
void ASURedBarrel::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASURedBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASURedBarrel::OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) {
	RadialForceComp->FireImpulse();
}

