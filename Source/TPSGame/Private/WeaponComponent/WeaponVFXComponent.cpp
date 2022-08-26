// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponComponent/WeaponVFXComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/DecalComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UWeaponVFXComponent::UWeaponVFXComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UWeaponVFXComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UWeaponVFXComponent::PlayFXImpact(const FHitResult& HitResult)
{
	if(!GetOwner())
		return;
	if(GetOwner()->HasAuthority())
	{
		PlayFXImpactMulticast(HitResult);
	}
	else
	{
		PlayFXImpactOnServer(HitResult);
	}
}

void UWeaponVFXComponent::PlayFXImpactOnServer_Implementation(const FHitResult& HitResult)
{
	PlayFXImpactMulticast(HitResult);
}

void UWeaponVFXComponent::PlayFXImpactMulticast_Implementation(const FHitResult& HitResult)
{
	if(!GetWorld())
		return;
	FImpactData ImpactData = DefaultImpactData;
	if(HitResult.PhysMaterial.IsValid())
	{
		const auto PhysicalMat = HitResult.PhysMaterial.Get();
		if(EffectsMap.Contains(PhysicalMat))
		{
			ImpactData = EffectsMap[PhysicalMat];
		}
	}
	//spawn niagara
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ImpactData.NiagaraSystem, HitResult.ImpactPoint, HitResult.ImpactNormal.Rotation());
	//Spawn decal
	auto Decal = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), ImpactData.WeaponDecalData.Material
		, ImpactData.WeaponDecalData.Size, HitResult.ImpactPoint, HitResult.ImpactNormal.Rotation());
	if(Decal)
	{
		Decal->SetFadeOut(ImpactData.WeaponDecalData.LifeTime, ImpactData.WeaponDecalData.FadeOutTime);
	}
}


