// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactables/Grabbable.h"
#include "Player/RatCharacter.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"


// Sets default values
AGrabbable::AGrabbable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootSceneComponent->SetupAttachment(RootComponent);
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComponent->SetupAttachment(RootSceneComponent);
	HitboxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Hitbox"));
	HitboxComponent->SetupAttachment(MeshComponent);

	InteractHitboxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractHitbox"));
	InteractHitboxComponent->SetupAttachment(MeshComponent);
	InteractHitboxComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Ignore);
	
	MeshComponent->SetSimulatePhysics(true);
  // HitboxComponent->SetNotifyRigidBodyCollision(true);
  // HitboxComponent->BodyInstance.bNotifyRigidBodyCollision = true;

}

// Called when the game starts or when spawned
void AGrabbable::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AGrabbable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGrabbable::Grab(ACharacter* GrabbingCharacter)
{
	// Update socket.
  // InteractHitboxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
  // AttachToActor(GrabbingCharacter, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true));
  
  // // Transform actor to appear as if carried by player.
  // SetActorLocation(GrabbingCharacter->GetActorLocation() + (GrabbingCharacter->GetActorForwardVector() * 100.f));
}


/**
 * TODO:
 * 	- Enable physics for dropping item.
 */
void AGrabbable::Drop()
{
	// DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

  // InteractHitboxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
            
  // // Physics w/ meshes is weird, so no physics, just place the item on the floor
  // FHitResult HitResult;            
  // FVector EndVector = GetActorLocation();
  // EndVector.Z -= 1000.f;
  // if (GetWorld()->LineTraceSingleByChannel(HitResult, GetActorLocation(), EndVector, ECollisionChannel::ECC_Visibility))
  // {
  //     SetActorLocation(HitResult.Location);
  // }
}
