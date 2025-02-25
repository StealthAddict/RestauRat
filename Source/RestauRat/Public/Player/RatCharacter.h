// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "InputActionValue.h"

#include "Interactables/Interactable.h"
#include "Interactables/Grabbable.h"

#include "RatCharacter.generated.h"


UCLASS()
class RESTAURAT_API ARatCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARatCharacter();

	// -- COMPONENTS -- 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* CameraComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Physics, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* HoldObjectComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Physics, meta = (AllowPrivateAccess = "true"))
	class UPhysicsConstraintComponent* GrabPhysicsConstraint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Physics, meta = (AllowPrivateAccess = "true"))
	class UArrowComponent* GrabPoint;

	// -- PROPERTIES --
	AInteractable* FocusedObject;
	AInteractable* HeldObject;

	// -- FUNCTIONS -- 
	virtual void Landed(const FHitResult& Hit) override;
	void DropObject();
	void GrabObject(AInteractable* ActorToGrab, UPrimitiveComponent* ComponentToGrab, FName ComponentName);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

private:
	FHitResult TriggerLineTrace();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
