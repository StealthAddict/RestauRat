// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactables/Interactable.h"
#include "Grabbable.generated.h"

/**
 * 
 */
UCLASS()
class RESTAURAT_API AGrabbable : public AInteractable
{
	GENERATED_BODY()

public:
	AGrabbable();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cooking")
	bool bIsCuttable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cooking")
	bool bIsIngredient;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cooking")
	FString ItemName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cooking")
	int32 NumCutPieces;

	UFUNCTION()
	void CutIngredient();

protected:
	// Set in BP
	UPROPERTY(EditDefaultsOnly, Category = "Cooking")
	TSubclassOf<AGrabbable> CutIngredientBlueprint;


};
