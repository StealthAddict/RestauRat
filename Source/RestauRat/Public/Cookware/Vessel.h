// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Vessel.generated.h"

/**
 * TODO:
 *  - On enter containingcollision, store ingredient objects in a list or what not
 * 	- Can do a dictionary with preset keys that are needed in the vessel for the recipe? Value is how many are in the vessel
 *  - On exit collision, remove from list
 */
UCLASS()
class RESTAURAT_API AVessel : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVessel();

	FORCEINLINE class USceneComponent* GetRootSceneComponent() const { return RootSceneComponent; }
	FORCEINLINE class UStaticMeshComponent* GetVesselMesh() const { return VesselMesh; }
	FORCEINLINE class UBoxComponent* GetContainingCollision() const { return ContainingCollision; }



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Vessel")
	class USceneComponent* RootSceneComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Vessel")
	class UStaticMeshComponent* VesselMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Vessel")
	class UBoxComponent* ContainingCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cooking")
	TMap<FString, int32> IngredientMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cooking")
	TMap<FString, int32> RecipeMap;

private:
	UFUNCTION()
	void OnContainerOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnContainerOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
