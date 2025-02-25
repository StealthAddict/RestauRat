// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/RatPlayerController.h"
#include "Player/RatCharacter.h"
#include "GameFramework/Controller.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interactables/Grabbable.h"

#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"


ARatPlayerController::ARatPlayerController()
{
    // -- CHARACTER STATE (ACTION) -- 
    CharacterActionState = ECharacterActionStateEnum::IDLE;
    Speed = 1.f;
    bIsRunning = false;
    bCanPop = true;
}

void ARatPlayerController::BeginPlay()
{
    Super::BeginPlay();

    if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
    {
        Subsystem->AddMappingContext(DefaultMappingContext, 0);
    }

}

// Called every frame
void ARatPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    ARatCharacter* Character = Cast<ARatCharacter>(this->GetCharacter());

    // Default back to idle
    if (CharacterActionState != ECharacterActionStateEnum::IDLE && CanPerformAction(ECharacterActionStateEnum::IDLE) && 
        (Character && !Character->GetCharacterMovement()->IsFalling()))
    { UpdateActionState(ECharacterActionStateEnum::IDLE); }

    
    if (Character && CharacterActionState == ECharacterActionStateEnum::POP && Character->GetCharacterMovement()->IsFalling())
    {
        Character->GetCharacterMovement()->GravityScale = 1.f;
    }
}

void ARatPlayerController::SetupInputComponent()
{
  Super::SetupInputComponent();

  if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
  {
    // Bind actions
    EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ARatPlayerController::OnLook);
    EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ARatPlayerController::OnMovePressed);
    EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &ARatPlayerController::OnSprintStart);
    EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ARatPlayerController::OnSprintStop);

    EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ARatPlayerController::OnJumpPressed);
    EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ARatPlayerController::OnJumpEnded);
    EnhancedInputComponent->BindAction(PopAction, ETriggerEvent::Started, this, &ARatPlayerController::OnPopPressed);

    EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ARatPlayerController::OnInteractPressed);
  }
}

/**
 * Verifies: Based on the last action, can this new one occur?
 */
bool ARatPlayerController::CanPerformAction(ECharacterActionStateEnum UpdatedAction)
{
    switch (CharacterActionState)
    {
        case ECharacterActionStateEnum::IDLE:
        case ECharacterActionStateEnum::MOVE:
        case ECharacterActionStateEnum::SPRINT:
        case ECharacterActionStateEnum::INTERACT:
            return true;
            break;
        case ECharacterActionStateEnum::JUMP:
        case ECharacterActionStateEnum::POP:
            if (UpdatedAction == ECharacterActionStateEnum::IDLE ||
               UpdatedAction == ECharacterActionStateEnum::MOVE)
            return true;
            break;
    }

    return false;
}

void ARatPlayerController::UpdateActionState(ECharacterActionStateEnum NewAction)
{
    ARatCharacter* Character = Cast<ARatCharacter>(this->GetCharacter());
    if (!Character) { return; }

    if (NewAction == ECharacterActionStateEnum::MOVE ||
        NewAction == ECharacterActionStateEnum::IDLE)
    {
        if ( abs(Character->GetVelocity().X + Character->GetVelocity().Y) <= 0.01f)
        {
            CharacterActionState = ECharacterActionStateEnum::IDLE;
        }
        else
        {
            CharacterActionState = ECharacterActionStateEnum::MOVE;
        }
    }
    else
    {
        CharacterActionState = NewAction;
    }
}



// -- ACTIONS -- 

void ARatPlayerController::OnLook(const FInputActionValue& Value)
{
  ARatCharacter* Character = Cast<ARatCharacter>(this->GetCharacter());
  if (Character) {
    // input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

    // add yaw and pitch input to controller
    Character->AddControllerYawInput(-LookAxisVector.X);
    Character->AddControllerPitchInput(LookAxisVector.Y);
  }
}

void ARatPlayerController::OnMovePressed(const FInputActionValue& Value)
{
    FVector2D MovementVector = Value.Get<FVector2D>();
    ARatCharacter* Character = Cast<ARatCharacter>(this->GetCharacter());
    if (Character && CanPerformAction(ECharacterActionStateEnum::MOVE)) {
        if (!MovementVector.IsNearlyZero())
        {
            UpdateActionState(ECharacterActionStateEnum::MOVE);
            // Get forward and right vectors
            const FRotator YawRotation(0, GetControlRotation().Yaw, 0);
            const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X) * Speed;
            const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y) * Speed;

            // Move the character forward and right based on input
            Character->AddMovementInput(ForwardDirection, MovementVector.Y);
            Character->AddMovementInput(RightDirection, MovementVector.X);

        }
    }
}

void ARatPlayerController::OnSprintStart()
{
    bIsRunning = true;
    ARatCharacter* Character = Cast<ARatCharacter>(this->GetCharacter());
    if (Character && CanPerformAction(ECharacterActionStateEnum::SPRINT)) {
        UpdateActionState(ECharacterActionStateEnum::SPRINT);
        Character->GetCharacterMovement()->MaxWalkSpeed = 850.f;
    }
}

void ARatPlayerController::OnSprintStop()
{
    bIsRunning = false;
    ARatCharacter* Character = Cast<ARatCharacter>(this->GetCharacter());
    if (Character) {
        Character->GetCharacterMovement()->MaxWalkSpeed = 500.f;
    }
}

void ARatPlayerController::OnJumpPressed()
{
    ARatCharacter* Character = Cast<ARatCharacter>(this->GetCharacter());
    if (Character && CanPerformAction(ECharacterActionStateEnum::JUMP)) {
        UpdateActionState(ECharacterActionStateEnum::JUMP);
        Character->Jump();
    }
}

void ARatPlayerController::OnJumpEnded()
{
    ARatCharacter* Character = Cast<ARatCharacter>(this->GetCharacter());
    if (Character) {
        Character->StopJumping();
    }
}

/**
 * A boosted jump with cooldown.
 */
void ARatPlayerController::OnPopPressed()
{
    ARatCharacter* Character = Cast<ARatCharacter>(this->GetCharacter());
    if (Character && CanPerformAction(ECharacterActionStateEnum::POP) && bCanPop) {
        UpdateActionState(ECharacterActionStateEnum::POP);

        Character->GetCharacterMovement()->AirControl = 0.20f;
        Character->GetCharacterMovement()->JumpZVelocity = 900.f;
        Character->GetCharacterMovement()->GravityScale = 0.8f;
        Character->Jump();

        bCanPop = false;
        GetWorldTimerManager().SetTimer(PopCooldownTimer, this, &ARatPlayerController::ResetPopCooldown, 5.f, false);
    }
}


/**
 * TODO:
 * - all of it 
 */
void ARatPlayerController::OnInteractPressed()
{
    ARatCharacter* Character = Cast<ARatCharacter>(this->GetCharacter());
    if (Character && CanPerformAction(ECharacterActionStateEnum::INTERACT)) {
        UpdateActionState(ECharacterActionStateEnum::INTERACT);
        // Check what object is currently infront and trigger its interact function.

        if (GEngine) {GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, TEXT("Interact Pressed"));}

        if (Character->HeldObject)
        {
            Character->DropObject();
        }
        else if (Character->FocusedObject) 
        {
            
            Character->FocusedObject->Interact(Character);
            Character->HeldObject = Character->FocusedObject;
            /** TODO: Test Physics Handle vs Physics Constraint */
        }
    }
}

