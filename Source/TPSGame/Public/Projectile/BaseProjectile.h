// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "WeaponComponent/WeaponVFXComponent.h"
#include "BaseProjectile.generated.h"

UCLASS()
class TPSGAME_API ABaseProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseProjectile();
	void SetShotDirection(const FVector& Direction){this->ShotDirection = Direction;};
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly, Category="Weapon")
	USphereComponent* CollisionComponent;

	UPROPERTY(VisibleDefaultsOnly, Category="Weapon")
	UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(VisibleDefaultsOnly, Category="Effect")
	UWeaponVFXComponent* VfxComponent;
	
	UPROPERTY(EditDefaultsOnly, Category="Amount")
	float DamageRadius = 200.0f;

	UPROPERTY(EditDefaultsOnly, Category="Amount")
	float DamageAmount = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category="Amount")
	bool DoFullDamage = true;

	UPROPERTY(EditDefaultsOnly, Category="Force")
	float Force = 500.0f;
private:
	FVector ShotDirection;
	UFUNCTION()
	void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	AController* GetController() const;
	
};
