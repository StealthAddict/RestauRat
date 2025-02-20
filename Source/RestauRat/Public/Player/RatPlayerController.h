// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"

#include "RatPlayerController.generated.h"


UENUM(BlueprintType)
enum class ECharacterActionStateEnum: uint8 {
    IDLE        UMETA(DisplayName = "Idling"),
    MOVE        UMETA(DisplayName = "Moving"),
		SPRINT      UMETA(DisplayName = "Sprinting"),
    JUMP        UMETA(DisplayName = "Jumping"),
		POP					UMETA(DisplayName = "Popping"),
    CROUCH      UMETA(DisplayName = "Crouching"),
    CLIMB       UMETA(DisplayName = "Climbing"),
    
    INTERACT    UMETA(DisplayName = "Interacting"),
		
		ATTACK     UMETA(DisplayName = "Attacking"),
    PULL        UMETA(DisplayName = "Pulling"),
    PICKUP      UMETA(DisplayName = "Pickup")
};

/**
 * 
 */
UCLASS()
class RESTAURAT_API ARatPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ARatPlayerController();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* SprintAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* PopAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* InteractAction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character State")
	ECharacterActionStateEnum CharacterActionState;

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	
	void OnLook(const FInputActionValue& Value);
	void OnMovePressed(const FInputActionValue& Value);
	void OnSprintStart();
	void OnSprintStop();
	void OnJumpPressed();
	void OnJumpEnded();

	void OnPopPressed();
	void OnInteractPressed();

	UFUNCTION(BlueprintCallable)
	virtual void UpdateActionState(ECharacterActionStateEnum NewAction);

	UFUNCTION(BlueprintCallable)
	bool CanPerformAction(ECharacterActionStateEnum UpdatedAction);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character State")
	float Speed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character State")
	bool bIsRunning;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character State")
	bool bCanPop;

private:
	FTimerHandle PopCooldownTimer;

	void ResetPopCooldown() { bCanPop = true; }
};

