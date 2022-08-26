// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NiagaraSystem.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "WeaponVFXComponent.generated.h"

USTRUCT(BlueprintType)
struct FWeaponDecalData
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Material")
	UMaterialInterface* Material;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Size")
	FVector Size = FVector(10.0f);
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="LifeTime")
	float LifeTime = 5.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="FadeOutTime")
	float FadeOutTime = 0.7f;
};

USTRUCT(BlueprintType)
struct FImpactData
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="DecalData")
	FWeaponDecalData WeaponDecalData;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="VFX")
	UNiagaraSystem* NiagaraSystem;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TPSGAME_API UWeaponVFXComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWeaponVFXComponent();
	
protected:
	UPROPERTY(EditDefaultsOnly, Category="VFX")
	FImpactData DefaultImpactData;
	UPROPERTY(EditDefaultsOnly, Category="VFX")
	TMap<UPhysicalMaterial*, FImpactData> EffectsMap;
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	void PlayFXImpact(const FHitResult& HitResult);

	UFUNCTION(Server, Reliable)
	void PlayFXImpactOnServer(const FHitResult& HitResult);
	UFUNCTION(NetMulticast, Reliable)
	void PlayFXImpactMulticast(const FHitResult& HitResult);
};
