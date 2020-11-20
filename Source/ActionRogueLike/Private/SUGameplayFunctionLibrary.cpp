// Fill out your copyright notice in the Description page of Project Settings.


#include "SUGameplayFunctionLibrary.h"
#include <SUAttributeComponent.h>

bool USUGameplayFunctionLibrary::ApplyDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount) {
	USUAttributeComponent* AttributeComp = USUAttributeComponent::GetAttributes(TargetActor);
	USUAttributeComponent* Causer = USUAttributeComponent::GetAttributes(DamageCauser);
	if (AttributeComp) {
		UE_LOG(LogTemp, Log, TEXT("AC DamMod: %f"), AttributeComp->GetCurrentDamageModifier())
		return AttributeComp->ApplyHealthChange(DamageCauser, -DamageAmount - Causer->GetCurrentDamageModifier());
	}
	return false;
}

bool USUGameplayFunctionLibrary::ApplyDirectionalDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount, const FHitResult& HitResult) {
	if (ApplyDamage(DamageCauser, TargetActor, DamageAmount)) {
		UPrimitiveComponent* HitComp = HitResult.GetComponent();
		if (HitComp && HitComp->IsSimulatingPhysics(HitResult.BoneName)) {
			FVector Direction = HitResult.TraceEnd - HitResult.TraceStart;
			Direction.Normalize();

			HitComp->AddImpulseAtLocation(Direction * 300000.0f, HitResult.ImpactPoint, HitResult.BoneName);
		}
		return true;
	}
	return false;
}