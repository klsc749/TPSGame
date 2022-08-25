// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/LauncherWeapon.h"

#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

void ALauncherWeapon::StartFire()
{
	Shot();
}

void ALauncherWeapon::SpawnProjectileOnServer_Implementation(const FVector& ShotDirection,const FVector& TraceEnd)
{
	SpawnProjectileMulticast(ShotDirection, TraceEnd);
}

void ALauncherWeapon::SpawnProjectileMulticast_Implementation(const FVector& ShotDirection,const FVector& TraceEnd)
{
	const FTransform SpawnTransform(FRotator::ZeroRotator, GetMuzzleWorldLocation());
	DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), TraceEnd, FColor::Red, false, 3.0f, 0, 3.0f);
	auto Projectile = GetWorld()->SpawnActorDeferred<ABaseProjectile>(ProjectileClass, SpawnTransform);
	if(!Projectile)
		return;
	//set projectile params
	Projectile->SetOwner(GetOwner());
	Projectile->SetShotDirection(ShotDirection);
	
	UGameplayStatics::FinishSpawningActor(Projectile, SpawnTransform);
}

void ALauncherWeapon::Shot()
{
	if(!GetWorld())
		return;
	//Get shot trace data
	FHitResult HitResult;
	FVector TraceStart, TraceEnd;
	if(!GetTraceData(TraceStart, TraceEnd))
		return;
	MakeHit(HitResult, TraceStart, TraceEnd);

	//determine the end point
	TraceEnd = HitResult.bBlockingHit ? HitResult.ImpactPoint : TraceEnd;
	//shot direction
	FVector ShotDirection = TraceEnd - GetMuzzleWorldLocation();
	//spawn the projectile
	/*const APawn* Player = Cast<APawn>(GetOwner());
	if(!Player)
		return;
	if (Player->HasAuthority())
	{
		SpawnProjectileMulticast(ShotDirection, TraceEnd);
		UE_LOG(LogTemp, Warning, TEXT("OnServer"))
	}
	else
	{
		SpawnProjectileOnServer(ShotDirection, TraceEnd);
		UE_LOG(LogTemp, Warning, TEXT("OnCilent"))
	}*/
	const FTransform SpawnTransform(FRotator::ZeroRotator, GetMuzzleWorldLocation());
	DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), TraceEnd, FColor::Red, false, 3.0f, 0, 3.0f);
	auto Projectile = GetWorld()->SpawnActorDeferred<ABaseProjectile>(ProjectileClass, SpawnTransform);
	if(!Projectile)
		return;
	//set projectile params
	Projectile->SetOwner(GetOwner());
	Projectile->SetShotDirection(ShotDirection);
	
	UGameplayStatics::FinishSpawningActor(Projectile, SpawnTransform);
}
