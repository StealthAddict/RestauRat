// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactables/Grabbable.h"

AGrabbable::AGrabbable()
{
  bIsCuttable = false;
  bIsIngredient = true;
  ItemName = "UNKNOWN";
  NumCutPieces = 2;
}


void AGrabbable::CutIngredient()
{
  if (bIsCuttable)
  {
    FVector Location = MeshComponent->GetComponentLocation();
    FRotator Rotation(0.0f, 0.0f, 0.0f);

    for (int i = 0; i < NumCutPieces; i++)
    {
      AGrabbable* CutPiece = GetWorld()->SpawnActor<AGrabbable>(CutIngredientBlueprint, Location, Rotation);
      Rotation.Yaw +=  360 / NumCutPieces;
    }

    Destroy();
  }
}