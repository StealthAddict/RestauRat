// Fill out your copyright notice in the Description page of Project Settings.


#include "Cookware/Vessel.h"
#include "Interactables/Grabbable.h"

#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"

// Sets default values
AVessel::AVessel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootSceneComponent->SetupAttachment(RootComponent);
	VesselMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	VesselMesh->SetupAttachment(RootSceneComponent);
	ContainingCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Hitbox"));
	ContainingCollision->SetupAttachment(VesselMesh);

	ContainingCollision->OnComponentBeginOverlap.AddDynamic(this, &AVessel::OnContainerOverlapBegin);
	ContainingCollision->OnComponentEndOverlap.AddDynamic(this, &AVessel::OnContainerOverlapEnd);
}

// Called when the game starts or when spawned
void AVessel::BeginPlay()
{
	Super::BeginPlay();
	
	
}

// Called every frame
void AVessel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AVessel::OnContainerOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (AGrabbable* GrabbableObject = Cast<AGrabbable>(OtherActor))
	{
		// if (GEngine) {GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Orange, FString::Printf(TEXT("%s ENTERED"), *GrabbableObject->ItemName));}
		
		if (GrabbableObject->bIsIngredient)
		{
			if (IngredientMap.Contains(GrabbableObject->ItemName))
			{ IngredientMap.Add(GrabbableObject->ItemName, IngredientMap[GrabbableObject->ItemName] + 1);	}
			else { IngredientMap.Add(GrabbableObject->ItemName, 1); }
			for (auto& Elem : IngredientMap)
			{
				GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Orange, *FString::Printf(TEXT("(%s, \"%d\")\n"), *Elem.Key, Elem.Value));
			}
		}
	}

}

void AVessel::OnContainerOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (AGrabbable* GrabbableObject = Cast<AGrabbable>(OtherActor))
	{
		if (GEngine) {GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Orange, FString::Printf(TEXT("%s EXITED"), *GrabbableObject->ItemName));}
		if (IngredientMap.Contains(GrabbableObject->ItemName))
	{ IngredientMap.Add(GrabbableObject->ItemName, IngredientMap[GrabbableObject->ItemName] - 1);	}
	}
	
	
}

