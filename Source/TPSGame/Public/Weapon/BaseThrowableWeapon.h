// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "BaseThrowableWeapon.generated.h"

UCLASS()
class TPSGAME_API ABaseThrowableWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseThrowableWeapon();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="MeshComponent");
	USkeletalMeshComponent* Mesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Movemeent")
	UProjectileMovementComponent* MovementComponent;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:	
};
