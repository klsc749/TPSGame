// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerHudWidget.h"

#include "HealthComponent/HealthComponent.h"
#include "WeaponComponent/WeaponComponent.h"

FString UPlayerHudWidget::GetBulletsInfo() const
{
	const auto Player = GetOwningPlayerPawn();
	if(!Player)
		return FString("");
	const auto Component = Player->GetComponentByClass(UWeaponComponent::StaticClass());
	const auto WeaponComponent = Cast<UWeaponComponent>(Component);
	if(!WeaponComponent)
		return  FString("");
	return WeaponComponent->GetBulletsInfo();
}

float UPlayerHudWidget::GetHealthPercent() const
{
	const auto Player = GetOwningPlayerPawn();
	if(!Player)
		return 0.0;
	const auto Component = Player->GetComponentByClass(UHealthComponent::StaticClass());
	const auto HealthComponent = Cast<UHealthComponent>(Component);
	if(!HealthComponent)
		return  0.0;
	return HealthComponent->GetHealthPercent();
}

bool UPlayerHudWidget::GetWeaponUIData(FWeaponUIData& WeaponUIData) const
{
	const auto Player = GetOwningPlayerPawn();
	if(!Player)
		return false;
	const auto Component = Player->GetComponentByClass(UWeaponComponent::StaticClass());
	const auto WeaponComponent = Cast<UWeaponComponent>(Component);
	if(!WeaponComponent)
		return  false;
	return WeaponComponent->GetWeaponUIData(WeaponUIData);
}
