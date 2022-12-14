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
	AudioComponent = CreateDefaultSubobject<UAudioComponent>("WeaponAudio");
	AudioComponent->SetComponentTickEnabled(false);
	AudioComponent->SetAutoActivate(false);
	SetRootComponent(WeaponMesh);
}

// Called when the game starts or when spawned
void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	CurrentBulletNum = WeaponAmmoData.BulletNum;
	SetCurrentBulletsNumInMag();
	LogWeaponMagData();
}

void ABaseWeapon::StartFire()
{
	Shot();
	GetWorldTimerManager().SetTimer(ShotTimerHandle, this, &ABaseWeapon::Shot, ShotInterval, true);
}

void ABaseWeapon::StopFire()
{
	GetWorldTimerManager().ClearTimer(ShotTimerHandle);
}

void ABaseWeapon::DecreaseCurrentBulletNumInMag()
{
	ChangeCurrentBulletNumInMag(-1);
}

void ABaseWeapon::ChangeCurrentBulletNumInMag(const int32& ChangeValue)
{
	CurrentBulletNumInMag += ChangeValue;
}

void ABaseWeapon::Reload()
{
	SetCurrentBulletsNumInMag();
}

void ABaseWeapon::ReloadBullet()
{
	SetCurrentBulletsNumInMag();
}

FString ABaseWeapon::GetBulletsInfo() const
{
	TArray<FStringFormatArg> Args;
	Args.Add(CurrentBulletNumInMag);
	Args.Add(CurrentBulletNum);
	return FString::Format(TEXT("{0}/{1}"), Args);
}

void ABaseWeapon::LogWeaponMagData() const
{
	UE_LOG(LogTemp, Warning, TEXT("%d/%d, Clip:%d"), CurrentBulletNumInMag, CurrentBulletNum, GetMagNum());
}

void ABaseWeapon::SetCurrentBulletsNumInMag()
{
	const int32 NeedToBeLoaded = WeaponAmmoData.BulletNumEachMag - CurrentBulletNumInMag;
	const int32 ChangeValue = NeedToBeLoaded < CurrentBulletNum ?
		NeedToBeLoaded : CurrentBulletNum;
	CurrentBulletNumInMag += ChangeValue;
	CurrentBulletNum -= ChangeValue;
}


void ABaseWeapon::Shot()
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
		DrawDebugLine(GetWorld(), SocketLocation, HitResult.ImpactPoint, FColor::Red, false, 3.0f, 0, 3.0f);
		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.0f, 24, FColor::Red, false, 5.0f);
	}
	else{
		DrawDebugLine(GetWorld(), SocketLocation, TraceEnd, FColor::Yellow, false, 3.0f, 0, 3.0f);
	}
	DecreaseCurrentBulletNumInMag();
	PlaySound(ShotSound);
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

void ABaseWeapon::PlaySound(USoundBase* SoundBase) const
{
	if(!SoundBase && !AudioComponent)
		return;
	AudioComponent->Sound = SoundBase;
	AudioComponent->Play();
}

bool ABaseWeapon::CheckCanShot()
{
	if(!CanShot())
	{
		StopFire();
		if(IsAmmoEmpty())
		{
			PlaySound(NoBulletSound);
		}
		else
		{
			OnClipEmpty.Broadcast();
			Reload();
		}
		return false;
	}
	return true;
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
	const int TraceMaxDistance = 2000;
	
	TraceStart = ViewLocation; //SocketTransform.GetLocation();
	const FVector ShootDirection = FMath::VRandCone(ViewRotation.Vector(), FMath::DegreesToRadians(1.5));
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
	CollisionQueryParams.bReturnPhysicalMaterial = true;
	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionQueryParams);
}

void ABaseWeapon::HideMag()
{
	const AController* PlayerController= GetPlayerController();
	if(!PlayerController)
		return;
	if(PlayerController->HasAuthority())
	{
		HideMagMulticast();
	}
	else
	{
		HideMagOnServer();
	}
}

void ABaseWeapon::UnHideMag()
{
	GetWeaponMesh()->HideBoneByName(MagBoneName, EPhysBodyOp::PBO_None);
	const AController* PlayerController= GetPlayerController();
	if(!PlayerController)
		return;
	if(PlayerController->HasAuthority())
	{
		UnHideMagMulticast();
	}
	else
	{
		UnHideMagOnServer();
	}
}

void ABaseWeapon::HideMagOnServer_Implementation()
{
	HideMagMulticast();
}

void ABaseWeapon::HideMagMulticast_Implementation()
{
	GetWeaponMesh()->UnHideBoneByName(MagBoneName);
}

void ABaseWeapon::UnHideMagOnServer_Implementation()
{
	UnHideMagMulticast();
}

void ABaseWeapon::UnHideMagMulticast_Implementation()
{
	GetWeaponMesh()->HideBoneByName(MagBoneName, EPhysBodyOp::PBO_None);
}

