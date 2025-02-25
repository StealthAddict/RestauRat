// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactables/Interactable.h"
#include "Player/RatCharacter.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "Components/ArrowComponent.h"



// Sets default values
AInteractable::AInteractable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootSceneComponent->SetupAttachment(RootComponent);
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComponent->SetupAttachment(RootSceneComponent);
	HitboxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Hitbox"));
	HitboxComponent->SetupAttachment(MeshComponent);

	MeshComponent->SetSimulatePhysics(true);

	bIsFocused = false;
}

// Called when the game starts or when spawned
void AInteractable::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInteractable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsFocused)
	{
		MeshComponent->SetOverlayMaterial(HighlightMaterial);
		bIsFocused = false;
	}
	else
	{
		MeshComponent->SetOverlayMaterial(nullptr);
	}

}

void AInteractable::Interact(ACharacter* Character)
{
	// Generic Interact
	if (GEngine) {GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Orange, TEXT("Triggered Interactable Object"));}

	
	ARatCharacter* RatCharacter = Cast<ARatCharacter>(Character);
	if (RatCharacter)
	{
		bIsFocused = false;
		// MoveComponentTo(RootSceneComponent, , RootSceneComponent->GetRelativeRotation(), RatCharacter->GrabPoint->GetComponentLocation(), true);
		RatCharacter->GrabObject(this, MeshComponent, "Mesh");
	}
}
