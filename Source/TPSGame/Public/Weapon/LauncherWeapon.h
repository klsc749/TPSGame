// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile/BaseProjectile.h"
#include "Weapon/BaseWeapon.h"
#include "LauncherWeapon.generated.h"

/**
 * 
 */
UCLASS()
class TPSGAME_API ALauncherWeapon : public ABaseWeapon
{
	GENERATED_BODY()
public:
    virtual void StartFire() override;

	UFUNCTION(Server, Reliable)
    void SpawnProjectileOnServer(const FVector& ShotDirection,const FVector& TraceEnd);
	UFUNCTION(NetMulticast, Reliable)
	void SpawnProjectileMulticast(const FVector& ShotDirection,const FVector& TraceEnd);
protected:
    UPROPERTY(EditDefaultsOnly, Category="Projectile")
    TSubclassOf<ABaseProjectile> ProjectileClass;
private:
	virtual void Shot() override;
};
