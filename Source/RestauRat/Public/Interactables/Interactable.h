// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Interactable.generated.h"

UCLASS()
class RESTAURAT_API AInteractable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractable();

	FORCEINLINE class USceneComponent* GetRootSceneComponent() const { return RootSceneComponent; }
	FORCEINLINE class UBoxComponent* GetHitboxComponent() const { return HitboxComponent; }
	FORCEINLINE class UStaticMeshComponent* GetMeshComponent() const { return MeshComponent; }
	
	virtual void Interact(ACharacter* Character);

	bool bIsFocused;

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

	// Set in BP
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interactable")
	UMaterialInterface* HighlightMaterial;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
