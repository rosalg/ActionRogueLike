// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SUInteractionComponent.generated.h"

class USUWorldUserWidget;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API USUInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	void PrimaryInteract();

public:	
	// Sets default values for this component's properties
	USUInteractionComponent();

protected:

	void FindBestInteractable();

	// Called when the game starts
	virtual void BeginPlay() override;

	// UPROP makes it so Unreal will clear object when it gets destroyed
	UPROPERTY()
	AActor* FocusedActor;

	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	float TraceDistance;

	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	float TraceRadius;

	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	TEnumAsByte<ECollisionChannel> CollisionChannel;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<USUWorldUserWidget> DefaultWidgetClass;

	UPROPERTY()
	USUWorldUserWidget* DefaultWidgetInstance;

// SERVER RELATED ONES!
protected:

	UFUNCTION(Server, Reliable)
	void ServerInteract(AActor* InFocus);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
