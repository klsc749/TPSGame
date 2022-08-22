// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Weapon/BaseWeapon.h"
#include "PlayerHudWidget.generated.h"

/**
 * 
 */
UCLASS()
class TPSGAME_API UPlayerHudWidget : public UUserWidget
{
	GENERATED_BODY()
	UFUNCTION(BlueprintCallable, Category="Bullets")
	FString GetBulletsInfo() const;
	UFUNCTION(BlueprintCallable, Category="Health")
	float GetHealthPercent() const;
	UFUNCTION(BlueprintCallable, Category="WeaponUI")
	bool GetWeaponUIData(FWeaponUIData& WeaponUIData) const;
};
