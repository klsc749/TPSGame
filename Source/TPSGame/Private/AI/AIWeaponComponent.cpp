// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AIWeaponComponent.h"

void UAIWeaponComponent::StartFire()
{
	if(!CanFire())
		return;
	if(CurrentWeapon->IsAmmoEmpty())
	{
		NextWeapon();
	}
	else
	{
		CurrentWeapon->StartFire();
	}
}

void UAIWeaponComponent::NextWeapon()
{
	if(!CanEquip())
	{
		return;
	}
	int32 NextWeaponIndex = (CurrentWeaponIndex + 1) % Weapons.Num();
	while (NextWeaponIndex != CurrentWeaponIndex)
	{
		if(!Weapons[NextWeaponIndex]->IsAmmoEmpty())
			break;
		NextWeaponIndex = (CurrentWeaponIndex + 1) % Weapons.Num();
	}

	if(CurrentWeaponIndex != NextWeaponIndex)
	{
		CurrentWeaponIndex = NextWeaponIndex;
		PlayAnimationMontage(EquipMontageAnim);
	}
}
