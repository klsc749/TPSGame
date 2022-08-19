// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Mag/Mag.h"
#include "BaseWeapon.generated.h"

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
	UAudioComponent* ShotAudio;
	
	UPROPERTY(VisibleAnywhere, Category="Ammo")
	int AmmoNum = 0;

	UPROPERTY(VisibleAnywhere, Category="Ammo")
	int ClipNum = 0;
	UPROPERTY(EditDefaultsOnly, Category="Ammo")
	int AmmoNumEachClip = 30;

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
	virtual void Shot();
	AController* GetPlayerController() const;
public:
	virtual void StartFire();
	virtual  void StopFire();
	inline int GetCurrentAmmoAmount() const {return AmmoNum;};
	inline FName GetEquipSocketName() const {return WeaponEquipSocket;}
	inline FName GetArmorySocketName() const {return WeaponArmoryName;}
	inline FName GetMagSocketName() const {return WeaponMagName;}
	void ChangeAmmoAmount(const int& ChangeValue);
	void ChangeClipAmount(const int& ChangeValue);
	FVector GetMuzzleWorldLocation() const;
	inline  USkeletalMeshComponent* GetWeaponMesh() const {return WeaponMesh;}
	virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const;
	virtual void MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd) const;
	void HideMag();
	void UnHideMag();
	inline TSubclassOf<AMag> GetMagClass() const {return Mag;}
private:
	
	FTimerHandle ShotTimerHandle;

	bool GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const;

};
