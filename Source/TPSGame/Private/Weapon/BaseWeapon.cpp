// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/BaseWeapon.h"

#include "DrawDebugHelpers.h"
#include "Components/AudioComponent.h"
#include "Player/BaseCharacter.h"

ABaseWeapon::ABaseWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
	WeaponAudio = CreateDefaultSubobject<UAudioComponent>("WeaponAudio");
	WeaponAudio->SetComponentTickEnabled(false);
	WeaponAudio->SetAutoActivate(false);
	SetRootComponent(WeaponMesh);
}

// Called when the game starts or when spawned
void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseWeapon::StartFire()
{
	Shot();
	GetWorldTimerManager().SetTimer(ShotTimerHandle, this, &ABaseWeapon::Shot, ShotInterval, true);
}

void ABaseWeapon::StopFire()
{
	GetWorldTimerManager().ClearTimer(ShotTimerHandle);
	UE_LOG(LogTemp, Warning, TEXT("Stop Shot"));
}

void ABaseWeapon::ChangeAmmoAmount(const int& ChangeValue)
{
	AmmoNum += ChangeValue;
	ChangeClipAmount(AmmoNum / AmmoNumEachClip);
	AmmoNum = FMath::Clamp(AmmoNum, 0, AmmoNumEachClip);
}

void ABaseWeapon::ChangeClipAmount(const int& ChangeValue)
{
	ClipNum = FMath::Clamp(ClipNum + ChangeValue, 0, MAX_int32);
}


void ABaseWeapon::Shot()
{
	UE_LOG(LogTemp, Warning, TEXT("Shot"));
	if(!GetWorld()){
		return;
	}
	const FVector SocketLocation = GetMuzzleWorldLocation();

	FVector TraceStart, TraceEnd;
	if(!GetTraceData(TraceStart, TraceEnd))
		return;
	
	FHitResult HitResult;
	MakeHit(HitResult, TraceStart, TraceEnd);
	if(HitResult.bBlockingHit){
		DrawDebugLine(GetWorld(), SocketLocation, HitResult.ImpactPoint, FColor::Red, false, 3.0f, 0, 3.0f);
		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.0f, 24, FColor::Red, false, 5.0f);
		MakeDamage(HitResult);
	}
	else{
		DrawDebugLine(GetWorld(), SocketLocation, TraceEnd, FColor::Yellow, false, 3.0f, 0, 3.0f);
	}

	WeaponAudio->Play();
}

AController* ABaseWeapon::GetPlayerController() const
{
	const ABaseCharacter* Player = Cast<ABaseCharacter>(GetOwner());
	if(!Player){
		return nullptr;
	}
    
	AController* Controller = Player->GetController();
	if(!Controller){
		return nullptr;
	}

	return Controller;
}

bool ABaseWeapon::GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const
{
	const AController* Controller = GetPlayerController();
	if(!Controller)
		return false;
	Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);
	return true;	
}

bool ABaseWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
{
	FVector ViewLocation;
	FRotator ViewRotation;
	if(!GetPlayerViewPoint(ViewLocation, ViewRotation))
		return false;
	const int TraceMaxDistance = 1000;
	
	TraceStart = ViewLocation; //SocketTransform.GetLocation();
	const FVector ShootDirection = ViewRotation.Vector();
	TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;

	return true;
}

FVector ABaseWeapon::GetMuzzleWorldLocation() const
{
	return WeaponMesh->GetSocketTransform(MuzzleSocketName).GetLocation();
}


void ABaseWeapon::MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd) const
{
	if(!GetWorld())
	{
		return;
	}
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(GetOwner());
	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionQueryParams);
}

void ABaseWeapon::MakeDamage(const FHitResult& HitResult)
{
	const auto DamageActor = HitResult.GetActor();
	if(!DamageActor)
		return;

	DamageActor->TakeDamage(DamageAmount, FDamageEvent{}, GetPlayerController(), this);
}

