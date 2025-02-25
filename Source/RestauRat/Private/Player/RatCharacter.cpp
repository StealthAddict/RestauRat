// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/RatCharacter.h"
#include "Player/RatPlayerController.h"
#include "Interactables/Interactable.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h" 

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"


// Sets default values
ARatCharacter::ARatCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// -- CAMERA -- 
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	GetCharacterMovement()->JumpZVelocity = 420.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	CameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// -- GRAB MECHANIC --
	HoldObjectComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HoldObjectComponent"));
	HoldObjectComponent->SetupAttachment(RootComponent);

	/** TODO: Determine wanted physics properties */
	GrabPhysicsConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("GrabPhysicsConstraint"));
	GrabPhysicsConstraint->SetupAttachment(HoldObjectComponent);
	GrabPhysicsConstraint->SetConstrainedComponents(GetCapsuleComponent(), "CollisionCylinder", nullptr, "None");

	GrabPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("GrabPoint"));
	GrabPoint->SetupAttachment(RootComponent);

	// -- PROPERTIES --
	FocusedObject = nullptr;
	HeldObject = nullptr;
}

// Called when the game starts or when spawned
void ARatCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (ARatPlayerController* PlayerController = Cast<ARatPlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
}

// Called every frame
void ARatCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Line trace
	if (!HeldObject)
	{
		FHitResult HitResult = TriggerLineTrace();
		
		// Highlight the focused actor 
		if (HitResult.GetActor()) 
		{
			/** Check if hit actor is of a class that is highlightable */
			AInteractable* HitInteractable = Cast<AInteractable>(HitResult.GetActor());
			if (!HitInteractable) { 
				/** Deselect the selected actor */
				if (FocusedObject)
				{
					FocusedObject->bIsFocused = false;
					FocusedObject = nullptr;	
				}
				return;
			}

			/** Select & save this new actor */
			FocusedObject = HitInteractable;
			HitInteractable->bIsFocused = true;
			
		} else if (FocusedObject)
		{  /** Deselect the selected actor */
			FocusedObject->bIsFocused = false;
			FocusedObject = nullptr;
		}
	}
	
}

// Called to bind functionality to input
void ARatCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

FHitResult ARatCharacter::TriggerLineTrace()
{
	// Line trace
	FHitResult HitResult;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	ETraceTypeQuery TraceChannel = UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility);
	FVector EndOfTrace = GetActorLocation() + (GetActorForwardVector() * 250);

	UKismetSystemLibrary::LineTraceSingle(GetWorld(), GetActorLocation(), EndOfTrace, // , 25.f
		TraceChannel, false, ActorsToIgnore, EDrawDebugTrace::None, HitResult, true, FColor::Red, FColor::Green);

	return HitResult;
}

/**
 * Character landed from a fall.
 * Reset jumping variables
 */
void ARatCharacter::Landed(const FHitResult& Hit) 
{
	Super::Landed(Hit);

	GetCharacterMovement()->JumpZVelocity = 420.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->GravityScale = 1.f;
}

/**
 * Drop item currently held.
 */
void ARatCharacter::DropObject()
{
	if (HeldObject)
	{
		if (GEngine) {GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Cyan, TEXT("Dropped!"));}
		HeldObject->bIsFocused = false;
		HeldObject = nullptr;     
		GrabPhysicsConstraint->BreakConstraint();
	}
}

/**
 * Move our HoldObjectComponent to the Grabbed Object and connect them with a physics constraint.
 */
void ARatCharacter::GrabObject(AInteractable* ActorToGrab, UPrimitiveComponent* ComponentToGrab, FName ComponentName)
{
	// Grab Object
	FHitResult HitResult = TriggerLineTrace();
	HoldObjectComponent->SetWorldLocationAndRotation(HitResult.ImpactPoint, HoldObjectComponent->GetComponentRotation(), false, nullptr, ETeleportType::TeleportPhysics);
	GrabPhysicsConstraint->SetConstrainedComponents(HoldObjectComponent, "HoldObjectComponent", HitResult.GetComponent(), HitResult.BoneName);
	HitResult.GetComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	// Reel object in
	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this;
	UKismetSystemLibrary::MoveComponentTo(HoldObjectComponent, GrabPoint->GetRelativeLocation(), HoldObjectComponent->GetRelativeRotation(), true, true, 0.3, true, EMoveComponentAction::Move, LatentInfo);
}