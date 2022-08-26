// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AICharacter.h"

#include "AI/AIWeaponComponent.h"
#include "AI/TPSAIController.h"
#include "GameFramework/CharacterMovementComponent.h"

AAICharacter::AAICharacter(const FObjectInitializer& ObjectInitializer ) :
Super(ObjectInitializer.SetDefaultSubobjectClass<UAIWeaponComponent>("WeaponComponent"))
{
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = ATPSAIController::StaticClass();

	bUseControllerRotationYaw = false;
	if(GetCharacterMovement())
	{
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 200.0f, 0.0f);
	}
}
