// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Weapon/BaseWeapon.h"
#include "WeaponComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TPSGAME_API UWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWeaponComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	int32 CurrentWeaponIndex = 0;

	UPROPERTY(VisibleAnywhere, Category="Weapon")
	ABaseWeapon* CurrentWeapon;

	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	TArray<ABaseWeapon*> Weapons;

	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	TArray<TSubclassOf<ABaseWeapon>> WeaponClasses;

	UPROPERTY(EditDefaultsOnly, Category="Animation")
	UAnimMontage* EquipMontage; 
	
public:	
	void SpawnWeapon();
	void StartFire();
	void StopFire();
	void EquipWeapon(int32 WeaponIndex);
	void NextWeapon();

private:
	void AttachWeaponToSocket(ABaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName) const;
	void PlayAnimationMontage(UAnimMontage* Animation);
		
};
