// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
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

	/**Animation**/
	UPROPERTY(EditDefaultsOnly, Category="Animation")
	UAnimMontage* EquipMontageAnim;

	UPROPERTY(EditDefaultsOnly, Category="Animation")
	UAnimMontage* ReloadMontageAnim;

	UPROPERTY(EditDefaultsOnly, Category="Audio")
	UAudioComponent* AudioComponent;

	UPROPERTY(EditDefaultsOnly, Category="Sound")
	USoundBase* ChangeWeaponSound;

	UPROPERTY(EditDefaultsOnly, Category="Sound")
	USoundBase* ChangeMagSound;
	
	UPROPERTY()
	AMag* CurrentMag;
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
public:	
	void SpawnWeapon();
	void StartFire();
	void StopFire();
	void EquipWeapon(int32 WeaponIndex);
	void NextWeapon();
	void Reload();
	bool GetWeaponUIData(FWeaponUIData& WeaponUIData) const;
	FString GetBulletsInfo() const;

private:
	void AttachActorToSocket(AActor* Actor, USceneComponent* SceneComponent, const FName& SocketName) const;
	void PlayAnimationMontage(UAnimMontage* Animation);
	void InitAnimations();
	bool CanEquip() const;
	bool CanFire() const;
	bool CanReload() const;
	bool CheckIsPlayer(const USkeletalMeshComponent* Mesh) const;
	void OnEquipFinished(const USkeletalMeshComponent* Mesh);
	void OnChangeWeapon(const USkeletalMeshComponent* Mesh);
	void OnReloadFinished(const USkeletalMeshComponent* Mesh);
	void OnChangeMag(const USkeletalMeshComponent* Mesh);
	void SpawnMag();
	void DestroyMag();
	void PlaySound(USoundBase* SoundBase) const;
	bool InEquipProgress = false;
	bool InReloadProgress = false;
};
