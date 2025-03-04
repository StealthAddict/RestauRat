// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactables/Knife.h"
#include "Interactables/Grabbable.h"
#include "Components/BoxComponent.h"

AKnife::AKnife()
{
  bIsCuttable = false;
  bIsIngredient = false;
  ItemName = "Knife";

  HurtboxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Hurtbox"));
	HurtboxComponent->SetupAttachment(MeshComponent);
  HurtboxComponent->OnComponentBeginOverlap.AddDynamic(this, &AKnife::OnObjectOverlap);
}

void AKnife::OnObjectOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
  if (AGrabbable* GrabbableActor = Cast<AGrabbable>(OtherActor))
  {
    if (GrabbableActor->bIsCuttable) 
    {
      GrabbableActor->CutIngredient();
    }
  }
}