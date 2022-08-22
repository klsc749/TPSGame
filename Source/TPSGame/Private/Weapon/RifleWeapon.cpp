// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/RifleWeapon.h"
#include "Components/AudioComponent.h"
#include "DrawDebugHelpers.h"
#include "Player/BaseCharacter.h"


ARifleWeapon::ARifleWeapon()
{
	WeaponVfxComponent = CreateDefaultSubobject<UWeaponVFXComponent>("WeaponFXComponent");	
}

void ARifleWeapon::BeginPlay()
{
	Super::BeginPlay();
	check(WeaponVfxComponent);
}

void ARifleWeapon::Shot()
{
	if(!GetWorld()){
		return;
	}
	if(!CheckCanShot())
		return;
	const FVector SocketLocation = GetMuzzleWorldLocation();

	FVector TraceStart, TraceEnd;
	if(!GetTraceData(TraceStart, TraceEnd))
		return;
	
	FHitResult HitResult;
	MakeHit(HitResult, TraceStart, TraceEnd);
	if(HitResult.bBlockingHit){
		WeaponVfxComponent->PlayFXImpact(HitResult);
		MakeDamage(HitResult);
	}
	PlaySound(ShotSound);
	DecreaseCurrentBulletNumInMag();
	PlayAnimation(FireAnim);
}

void ARifleWeapon::StartFire()
{
	Shot();
	GetWorldTimerManager().SetTimer(ShotTimerHandle, this, &ARifleWeapon::Shot, ShotInterval, true);
}

void ARifleWeapon::StopFire()
{
	GetWorldTimerManager().ClearTimer(ShotTimerHandle);
}

void ARifleWeapon::MakeDamage(const FHitResult& HitResult)
{
	const auto DamageActor = HitResult.GetActor();
	if(!DamageActor)
		return;

	DamageActor->TakeDamage(DamageAmount, FDamageEvent{}, GetPlayerController(), this);
}

void ARifleWeapon::PlayAnimation(UAnimMontage* FireAnimation)
{
	ACharacter* Player = Cast<ACharacter>(GetOwner());
	if(!Player && !FireAnimation)
		return;
	Player->PlayAnimMontage(FireAnimation);
}


