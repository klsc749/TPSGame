// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnDeath)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHealthChange, float)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TPSGAME_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

	float GetHealth() const {return Health;}

	void SetHealth(const float NewHealth);

	void ChangeHealth(float HealthChange);

	UFUNCTION(BlueprintCallable)
	bool IsDead() const {return Health <= 0.0f;}

	float GetHealthPercent() const {return Health / MaxHealth;}

	FOnDeath OnDeath;

	FOnHealthChange OnHealthChange;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MaxHealth = 100.0f;

	UPROPERTY(EditDefaultsOnly, BluePrintReadWrite, Category = "Heal")
	bool AutoHeal = true;
	
	UPROPERTY(EditDefaultsOnly, BluePrintReadWrite, Category = "Heal", meta = (EditCondition = "AutoHeal"))
	float HealUpdateTime = 1.0f;
	
	UPROPERTY(EditDefaultsOnly, BluePrintReadWrite, Category = "Heal", meta = (EditCondition = "AutoHeal"))
	float HealDelay = 3.0f;
	
	UPROPERTY(EditDefaultsOnly, BluePrintReadWrite, Category = "Heal", meta = (EditCondition = "AutoHeal"))
	float HealModifier = 5.0f;

	UFUNCTION()
	void OnTakeAnyDamageHandle(AActor* DamageActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	void HealUpdate();
private:
	float Health = 0.0f;
	FTimerHandle HealTimerHandle;
};
