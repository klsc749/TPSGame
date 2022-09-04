// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/SplineComponent.h"
#include "Projectile/BaseProjectile.h"
#include "Weapon/BaseWeapon.h"
#include "WeaponComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TPSGAME_API UWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWeaponComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	/**Weapon Info**/
	int32 CurrentWeaponIndex = 0;

	UPROPERTY(VisibleAnywhere, Category="Weapon")
	ABaseWeapon* CurrentWeapon;

	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	TArray<ABaseWeapon*> Weapons;

	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	TArray<TSubclassOf<ABaseWeapon>> WeaponClasses;

	UPROPERTY(EditDefaultsOnly, Category="Throw Speed")
	float ThrowSpeed = 1200.0f;
	/**Animation**/
	UPROPERTY(EditDefaultsOnly, Category="Animation")
	UAnimMontage* EquipMontageAnim;

	UPROPERTY(EditDefaultsOnly, Category="Animation")
	UAnimMontage* ReloadMontageAnim;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="ANimation")
	UAnimMontage* PreThrow;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="ANimation")
	UAnimMontage* AimIdle;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="ANimation")
	UAnimMontage* ThrowMontage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="SocketName")
	FName ThrowSocketName;
	/***Audio****/

	UPROPERTY(EditDefaultsOnly, Category="Sound")
	USoundBase* ChangeWeaponSound;

	UPROPERTY(EditDefaultsOnly, Category="Sound")
	USoundBase* ChangeMagSound;

	UPROPERTY(EditDefaultsOnly, Category="Grenade")
	TSubclassOf<ABaseProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category="ThrowPath")
	float ThrowPathGravity = 5.0f;
	
	UPROPERTY()
	AMag* CurrentMag;
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
public:	
	void SpawnWeapon();
	virtual void StartFire();
	void StopFire();
	void EquipWeapon(int32 WeaponIndex);
	virtual void NextWeapon();
	void Reload();
	bool GetWeaponUIData(FWeaponUIData& WeaponUIData) const;
	FString GetBulletsInfo() const;
	void BeginThrow();
	void FinishThrow();
	void OnFinishPreThrow(const USkeletalMeshComponent* Mesh);

protected:
	bool CanEquip() const;
	bool CanFire() const;
	bool CanReload() const;
	void PlayAnimationMontage(UAnimMontage* Animation);
private:
	void AttachActorToSocket(AActor* Actor, USceneComponent* SceneComponent, const FName& SocketName) const;

	void InitAnimations();
	bool CanThrow() const;
	bool CheckIsPlayer(const USkeletalMeshComponent* Mesh) const;
	void OnEquipFinished(const USkeletalMeshComponent* Mesh);
	void OnChangeWeapon(const USkeletalMeshComponent* Mesh);
	void OnReloadFinished(const USkeletalMeshComponent* Mesh);
	void OnChangeMag(const USkeletalMeshComponent* Mesh);
	void OnFinishThrow(const USkeletalMeshComponent* Mesh);
	void SpawnMag();
	void DestroyMag();
	void PlaySound(USoundBase* SoundBase) const;
	bool InEquipProgress = false;
	bool InReloadProgress = false;
	bool InThrowProgress = false;
	FTimerHandle TraceTimerHandle;
	void MakeTrace();
	void SpawnGrenade();

public:
	UFUNCTION(Server, Reliable)
	void StartFireOnServer();
	UFUNCTION(NetMulticast, Reliable)
	void StartFireMulticast();
	UFUNCTION(Server, Reliable)
	void EndFireOnServer();
	UFUNCTION(NetMulticast, Reliable)
	void EndFireMulticast();
	UFUNCTION(Server, Reliable)
	void ReloadOnServer();
	UFUNCTION(NetMulticast, Reliable)
	void ReloadMultiCast();
	UFUNCTION(Server, Reliable)
	void ChangeWeaponOnServer();
	UFUNCTION(NetMulticast, Reliable)
	void ChangeWeaponMulticast();
};
