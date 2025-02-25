// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Grabbable.generated.h"

UCLASS()
class RESTAURAT_API AGrabbable : public AActor
{
	GENERATED_BODY()
	
	/**
	 * TODO:
	 * 	- Change to be a child of Interactable
	 */

public:	
	// Sets default values for this actor's properties
	AGrabbable();

	FORCEINLINE class USceneComponent* GetRootSceneComponent() const { return RootSceneComponent; }
	FORCEINLINE class UBoxComponent* GetHitboxComponent() const { return HitboxComponent; }
	FORCEINLINE class UStaticMeshComponent* GetMeshComponent() const { return MeshComponent; }
	FORCEINLINE class UBoxComponent* GetInteractHitboxComponent() const { return InteractHitboxComponent; }
	
	virtual void Grab(ACharacter* GrabbingCharacter);
	virtual void Drop();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Add Blueprint accessible Components to define the Mesh and the Hit Box
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Interactable")
	class USceneComponent* RootSceneComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Interactable")
	class UBoxComponent* HitboxComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Interactable")
	class UStaticMeshComponent* MeshComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Interactable")
	class UBoxComponent* InteractHitboxComponent;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
