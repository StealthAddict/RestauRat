// Copyright Epic Games, Inc. All Rights Reserved.


#include "RestauRatGameModeBase.h"
#include "UObject/ConstructorHelpers.h"

ARestauRatGameModeBase::ARestauRatGameModeBase() {
    
  static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/BP_RatPawn"));
  if (PlayerPawnBPClass.Class != nullptr)
  {
      DefaultPawnClass = PlayerPawnBPClass.Class;
  }
}