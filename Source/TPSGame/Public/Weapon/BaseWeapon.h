// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Mag/Mag.h"
#include "BaseWeapon.generated.h"

USTRUCT()
struct FWeaponMagData
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly, Category="Mag")
	int32 BulletNum;
	UPROPERTY(EditDefaultsOnly, Category="Mag")
	int32 BulletNumEachMag;
};

USTRUCT(BlueprintType)
struct FWeaponUIData
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Icon")
	UTexture2D* WeaponIcon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Icon")
	UTexture2D* CrossHair;
};

DECLARE_MULTICAST_DELEGATE(FOnClipEmpty);

UCLASS()
class TPSGAME_API ABaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category="Mesh")
	USkeletalMeshComponent* WeaponMesh;
	
	UPROPERTY(EditDefaultsOnly, Category="Audio")
	UAudioComponent* AudioComponent;

	UPROPERTY(EditDefaultsOnly, Category="Audio")
	USoundBase* ShotSound;

	UPROPERTY(EditDefaultsOnly, Category="Audio")
	USoundBase* NoBulletSound;
	
	UPROPERTY(EditDefaultsOnly, Category="Ammo")
	FWeaponMagData WeaponAmmoData{16,  3};

	UPROPERTY(EditDefaultsOnly, Category="Ammo")
	bool IsAmmoInfinite = false;

	UPROPERTY(EditDefaultsOnly, Category="Shot")
	float ShotInterval = 0.2f;

	UPROPERTY(EditDefaultsOnly, Category="Socket")
	FName MuzzleSocketName = "Muzzle";
	
	UPROPERTY(EditDefaultsOnly, Category="Damage")
	float DamageAmount = 4.0f;

	UPROPERTY(EditDefaultsOnly, Category="Socket")
	FName WeaponEquipSocket = "WeaponSocket";

	UPROPERTY(EditDefaultsOnly, Category="Socket")
	FName WeaponArmoryName = "ArmorySocket";

	UPROPERTY(EditDefaultsOnly, Category="Socket")
	FName WeaponMagName = "MagSocket";

	UPROPERTY(EditDefaultsOnly, Category="Mag")
	FName MagBoneName = "b_gun_mag";

	UPROPERTY(EditDefaultsOnly, Category="Mag")
	TSubclassOf<AMag> Mag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="UI")
	FWeaponUIData WeaponUIData;
	virtual void Shot();
	AController* GetPlayerController() const;
	bool CanShot() const {return  CurrentBulletNumInMag != 0;}
	void PlaySound(USoundBase* SoundBase) const;
	bool CheckCanShot();
public:
	FOnClipEmpty OnClipEmpty;
	virtual void StartFire();
	virtual  void StopFire();
	void DecreaseCurrentBulletNumInMag();
	inline int GetCurrentAmmoAmount() const {return CurrentBulletNum;};
	inline FName GetEquipSocketName() const {return WeaponEquipSocket;}
	inline FName GetArmorySocketName() const {return WeaponArmoryName;}
	inline FName GetMagSocketName() const {return WeaponMagName;}
	FVector GetMuzzleWorldLocation() const;
	inline  USkeletalMeshComponent* GetWeaponMesh() const {return WeaponMesh;}
	virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const;
	virtual void MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd) const;
	void HideMag() const;
	void UnHideMag() const;
	void Reload();
	void ReloadBullet();
	inline TSubclassOf<AMag> GetMagClass() const {return Mag;}
	inline bool IsAmmoEmpty() const {return CurrentBulletNum == 0;}
	inline int32 GetMagNum() const {return CurrentBulletNum / WeaponAmmoData.BulletNumEachMag;}
	FString GetBulletsInfo() const;
	inline FWeaponUIData GetWeaponUIData() const {return  WeaponUIData;};
private:
	int32 CurrentBulletNum;
	int32 CurrentBulletNumInMag;
	FTimerHandle ShotTimerHandle;

	bool GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const;
	void ChangeCurrentBulletNumInMag(const int32& ChangeValue);
	void LogWeaponMagData() const;
	void SetCurrentBulletsNumInMag();
};
