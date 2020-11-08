// Fill out your copyright notice in the Description page of Project Settings.


#include "SUInteractionComponent.h"
#include <SUItemChest.h>
#include <DrawDebugHelpers.h>
#include "..\Public\SUInteractionComponent.h"
#include "SUWorldUserWidget.h"

static TAutoConsoleVariable<bool> CVarDebugDrawInteraction(TEXT("su.InteractionDebugDraw"), false, TEXT("Enable Debug Lines for Interact Comp"), ECVF_Cheat);

// Sets default values for this component's properties
USUInteractionComponent::USUInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	TraceRadius = 30.0f;
	TraceDistance = 500.0f;
	CollisionChannel = ECC_WorldDynamic;
}


// Called when the game starts
void USUInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}



// Called every frame
void USUInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	APawn* MyPawn = Cast<APawn>(GetOwner());
	if (MyPawn->IsLocallyControlled()) {
		FindBestInteractable();
	}

}

void USUInteractionComponent::FindBestInteractable()
{
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(CollisionChannel);

	AActor* MyOwner = GetOwner();


	FVector EyeLocation;
	FRotator EyeRotation;
	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	FVector End = EyeLocation + (EyeRotation.Vector() * TraceDistance);

	//FHitResult Hit;
	//GetWorld()->LineTraceSingleByObjectType(Hit, EyeLocation, End, ObjectQueryParams);

	// Clear ref before trying to fill
	FocusedActor = nullptr;

	TArray<FHitResult> Hits;
	FCollisionShape Shape;
	float Radius = TraceRadius;
	Shape.SetSphere(Radius);

	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, End, FQuat::Identity, ObjectQueryParams, Shape);

	FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;

	for (FHitResult Hit : Hits) {
		AActor* HitActor = Hit.GetActor();
		if (CVarDebugDrawInteraction.GetValueOnGameThread()) DrawDebugSphere(GetWorld(), Hit.ImpactPoint, Radius, 32, LineColor, false, 2.0f);
		if (HitActor) {
			if (HitActor->Implements<USUGameplayInterface>()) {
				FocusedActor = HitActor;
				break;
			}
		}
	}

	if (FocusedActor) {
		//This is a lazy load.
		if (DefaultWidgetInstance == nullptr && ensure(DefaultWidgetClass)) {
			DefaultWidgetInstance = CreateWidget<USUWorldUserWidget>(GetWorld(), DefaultWidgetClass);
		}
		
		if (DefaultWidgetInstance) {
			DefaultWidgetInstance->AttachedActor = FocusedActor;
			if (!DefaultWidgetInstance->IsInViewport()) {
				DefaultWidgetInstance->AddToViewport();
			}
		}
	}
	else {
		if (DefaultWidgetInstance) {
			DefaultWidgetInstance->RemoveFromParent();
		}
	}

	if (CVarDebugDrawInteraction.GetValueOnGameThread()) DrawDebugLine(GetWorld(), EyeLocation, End, LineColor, false, 2.0f, 0, 2.0f);
}

// Focused actor is what I, the client or server know as the thing in focus.
// Server Interact says server, interact with the object passed in. 
void USUInteractionComponent::PrimaryInteract() {
	ServerInteract(FocusedActor);
}

void USUInteractionComponent::ServerInteract_Implementation(AActor* InFocus)
{
	if (InFocus == nullptr) {
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "No Focus Actor to interact.");
		return;
	}

	APawn* MyPawn = Cast<APawn>(GetOwner());

	ISUGameplayInterface::Execute_Interact(InFocus, MyPawn);
}