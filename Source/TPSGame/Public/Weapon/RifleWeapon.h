// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/BaseWeapon.h"
#include "RifleWeapon.generated.h"

/**
 * 
 */
UCLASS()
class TPSGAME_API ARifleWeapon : public ABaseWeapon
{
	GENERATED_BODY()
protected:

	UPROPERTY(EditDefaultsOnly, Category="Animation")
	UAnimMontage* FireAnim;
	
	virtual void Shot() override;
	virtual void StartFire() override;
	virtual void StopFire() override;
private:
	void MakeDamage(const FHitResult& HitResult);
	void PlayAnimation(UAnimMontage* FireAnimation);
	FTimerHandle ShotTimerHandle;
};

