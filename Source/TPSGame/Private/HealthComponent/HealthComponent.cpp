// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent/HealthComponent.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UHealthComponent::SetHealth(const float NewHealth)
{
	this->Health = FMath::Clamp(NewHealth, 0.0f, MaxHealth);
	OnHealthChange.Broadcast(this->Health);
}

void UHealthComponent::ChangeHealth(float HealthChange)
{
	SetHealth(this->Health + HealthChange);
}

void UHealthComponent::OnTakeAnyDamageHandle(AActor* DamageActor, float Damage, const UDamageType* DamageType,
	AController* InstigatedBy, AActor* DamageCauser)
{
	if(Damage < 0 || IsDead() || !GetWorld())
	{
		return;
	}
	ChangeHealth(-Damage);

	GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);
	if(IsDead())
	{
		OnDeath.Broadcast();
	}else
	{
		GetWorld()->GetTimerManager().SetTimer(HealTimerHandle, this, &UHealthComponent::HealUpdate, HealUpdateTime, true, HealDelay);
	}
}

void UHealthComponent::HealUpdate()
{
	if(!GetWorld())
		return;
	ChangeHealth(HealModifier);
	if(FMath::IsNearlyEqual(GetHealth(), MaxHealth) && GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);
	}
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	// ...
	SetHealth(MaxHealth);
	AActor* Owner = GetOwner();
	if(Owner)
	{
		Owner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::OnTakeAnyDamageHandle);
	}
}


