// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactables/Grabbable.h"
#include "Knife.generated.h"

/**
 * 
 */
UCLASS()
class RESTAURAT_API AKnife : public AGrabbable
{
	GENERATED_BODY()
	
public:
	AKnife();

	FORCEINLINE class UBoxComponent* GetHurtboxComponent() const { return HurtboxComponent; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Interactable")
	class UBoxComponent* HurtboxComponent;

private:
	UFUNCTION()
	void OnObjectOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
