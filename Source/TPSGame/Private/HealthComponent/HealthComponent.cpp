// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent/HealthComponent.h"

#include "Player/TPSPlayerController.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicated(true);
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
	const APawn* Player = Cast<APawn>(GetOwner());
	if(!Player)
		return;
	if(Player->HasAuthority())
	{
		TakeDamageHandleMulticast(Damage);
	}else
	{
		TakeDamageHandleOnServer(Damage);
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

void UHealthComponent::TakeDamageHandleOnServer_Implementation(float Damage)
{
	TakeDamageHandleOnServer(Damage);
}

void UHealthComponent::TakeDamageHandleMulticast_Implementation(float Damage)
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


