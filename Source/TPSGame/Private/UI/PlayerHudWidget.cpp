// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerHudWidget.h"

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

bool UPlayerHudWidget::IsPlayerAlive() const
{
	const auto HealthComponent = GetHealthComponent();
	return HealthComponent && !HealthComponent->IsDead();
}

bool UPlayerHudWidget::IsPlayerSpectating() const
{
	const auto Controller = GetOwningPlayer();
	return Controller && Controller->GetStateName() == NAME_Spectating;
}

UHealthComponent* UPlayerHudWidget::GetHealthComponent() const
{
	const auto Player = GetOwningPlayerPawn();
	if(!Player)
		return nullptr;
	const auto Component = Player->GetComponentByClass(UHealthComponent::StaticClass());
	const auto HealthComponent = Cast<UHealthComponent>(Component);
	return HealthComponent;
}

UWeaponComponent* UPlayerHudWidget::GetWeaponComponent() const
{
	const auto Player = GetOwningPlayerPawn();
	if(!Player)
		return nullptr;
	const auto Component = Player->GetComponentByClass(UWeaponComponent::StaticClass());
	const auto WeaponComponent = Cast<UWeaponComponent>(Component);
	return WeaponComponent;
}
