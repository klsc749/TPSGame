// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponComponent/WeaponComponent.h"

#include "AnimNotify/ChangeMagAnimNotify.h"
#include "AnimNotify/ChangeWeaponAnimNotify.h"
#include "AnimNotify/EquipFinishAnimNotify.h"
#include "AnimNotify/ReloadFinishAnimNotify.h"
#include "Components/AudioComponent.h"
#include "Player/BaseCharacter.h"

// Sets default values for this component's properties
UWeaponComponent::UWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	AudioComponent = CreateDefaultSubobject<UAudioComponent>("ChangeWeapon");
	AudioComponent->SetAutoActivate(false);
	AudioComponent->SetComponentTickEnabled(false);
	// ...
}


// Called when the game starts
void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	InitAnimations();
	CurrentWeaponIndex = 0;
	SpawnWeapon();
	EquipWeapon(CurrentWeaponIndex);
}

void UWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	CurrentWeapon = nullptr;
	for(auto Weapon : Weapons)
	{
		Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		Weapon->Destroy();
	}
	Weapons.Empty();
}

void UWeaponComponent::SpawnWeapon()
{
	if(!GetWorld())
		return;

	ABaseCharacter* Character = Cast<ABaseCharacter>(GetOwner());
	if(!Character)
		return;
	for(auto WeaponClass : WeaponClasses)
	{
		auto Weapon = GetWorld()->SpawnActor<ABaseWeapon>(WeaponClass);
		if(!Weapon)
			continue;
		Weapon->SetOwner(Character);
		Weapon->OnClipEmpty.AddUObject(this, &UWeaponComponent::Reload);
		Weapons.Add(Weapon);
		AttachActorToSocket(Weapon, Character->GetMesh(), Weapon->GetArmorySocketName());
	}
}

void UWeaponComponent::StartFire()
{
	if(!CanFire())
		return;
	if(!CurrentWeapon)
		return;
	CurrentWeapon->StartFire();
}

void UWeaponComponent::StopFire()
{
	if(!CurrentWeapon)
		return;
	CurrentWeapon->StopFire();
}

void UWeaponComponent::EquipWeapon(int32 WeaponIndex)
{
	if(WeaponIndex < 0 || WeaponIndex > Weapons.Num() || Weapons.Num() == 0)
		return;
	ABaseCharacter* Character = Cast<ABaseCharacter>(GetOwner());
	if(!Character)
		return;

	if(CurrentWeapon)
	{
		AttachActorToSocket(CurrentWeapon, Character->GetMesh(), CurrentWeapon->GetArmorySocketName());
	}
	
	CurrentWeapon = Weapons[WeaponIndex];
	AttachActorToSocket(CurrentWeapon, Character->GetMesh(), CurrentWeapon->GetEquipSocketName());
}

void UWeaponComponent::NextWeapon()
{
	if(!CanEquip())
		return;
	if(!EquipMontageAnim)
		return;
	if(Weapons.Num() == 0)
		return;
	InEquipProgress = true;
	CurrentWeaponIndex = (CurrentWeaponIndex + 1) % Weapons.Num();
	PlayAnimationMontage(EquipMontageAnim);
}

void UWeaponComponent::Reload()
{
	if(!CanReload())
		return;
	if(!ReloadMontageAnim)
		return;
	InReloadProgress = true;
	CurrentWeapon->Reload();
	PlayAnimationMontage(ReloadMontageAnim);
}

bool UWeaponComponent::GetWeaponUIData(FWeaponUIData& WeaponUIData) const
{
	if(CurrentWeapon)
	{
		WeaponUIData = CurrentWeapon->GetWeaponUIData();
		return true;
	}
	return false;
}

FString UWeaponComponent::GetBulletsInfo() const
{
	if(!CurrentWeapon)
		return FString("");
	return CurrentWeapon->GetBulletsInfo();
}

void UWeaponComponent::AttachActorToSocket(AActor* Actor, USceneComponent* SceneComponent,
                                           const FName& SocketName) const
{
	if(!Actor || !SceneComponent)
		return;
	const FAttachmentTransformRules AttachmentTransformRules(EAttachmentRule::SnapToTarget, false);
	Actor->AttachToComponent(SceneComponent, AttachmentTransformRules, SocketName);
}

void UWeaponComponent::PlayAnimationMontage(UAnimMontage* Animation)
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if(!Character)
		return;
	Character->PlayAnimMontage(Animation);
}

void UWeaponComponent::PlaySound(USoundBase* SoundBase) const
{
	if(!AudioComponent && !SoundBase)
		return;
	AudioComponent->Sound = SoundBase;
	AudioComponent->Play();
}

void UWeaponComponent::InitAnimations()
{
	if(!EquipMontageAnim)
		return;
	TArray<FAnimNotifyEvent> Notifies = EquipMontageAnim->Notifies;
	for(const FAnimNotifyEvent& NotifyEvent : Notifies)
	{
		UEquipFinishAnimNotify* EquipFinishAnimNotify = Cast<UEquipFinishAnimNotify>(NotifyEvent.Notify);
		if(EquipFinishAnimNotify)
		{
			EquipFinishAnimNotify->OnNotified.AddUObject(this, &UWeaponComponent::OnEquipFinished);
			break;
		}
	}

	for(const FAnimNotifyEvent& NotifyEvent : Notifies)
	{
		UChangeWeaponAnimNotify* ChangeWeaponAnimNotify = Cast<UChangeWeaponAnimNotify>(NotifyEvent.Notify);
		if(ChangeWeaponAnimNotify)
		{
			ChangeWeaponAnimNotify->OnNotified.AddUObject(this, &UWeaponComponent::OnChangeWeapon);
			break;
		}
	}
	if(!ReloadMontageAnim)
		return;
	Notifies = ReloadMontageAnim->Notifies;
	for(const FAnimNotifyEvent& NotifyEvent : Notifies)
	{
		UReloadFinishAnimNotify* ReloadFinishAnimNotify = Cast<UReloadFinishAnimNotify>(NotifyEvent.Notify);
		if(ReloadFinishAnimNotify)
		{
			UE_LOG(LogTemp, Warning, TEXT("ReloadFinishAnimNotify %p"), ReloadFinishAnimNotify);
			ReloadFinishAnimNotify->OnNotified.AddUObject(this, &UWeaponComponent::OnReloadFinished);
			break;
		}
	}

	for(const FAnimNotifyEvent& NotifyEvent : Notifies)
	{
		UChangeMagAnimNotify* ChangeMagAnimNotify = Cast<UChangeMagAnimNotify>(NotifyEvent.Notify);
		UE_LOG(LogTemp, Warning, TEXT("Bind %p"), ChangeMagAnimNotify);
		if(ChangeMagAnimNotify)
		{
			ChangeMagAnimNotify->OnNotified.AddUObject(this, &UWeaponComponent::OnChangeMag);
			break;
		}
	}
}

bool UWeaponComponent::CanEquip() const
{
	return !InEquipProgress && !InReloadProgress;
}

bool UWeaponComponent::CanFire() const
{
	if(!CurrentWeapon)
		return false;
	const ABaseCharacter* Player = Cast<ABaseCharacter>(GetOwner());
	if(!Player)
		return false;
	return CurrentWeapon && !Player->IsSprinting() && !InEquipProgress && !InReloadProgress;
}

bool UWeaponComponent::CanReload() const
{
	return !InEquipProgress && !InReloadProgress;
}

bool UWeaponComponent::CheckIsPlayer(const USkeletalMeshComponent* Mesh) const
{
	const ACharacter* Character = Cast<ACharacter>(GetOwner());
	if(!Character)
		return false;
	if(Character->GetMesh() != Mesh)
		return false;
	return true;
}

void UWeaponComponent::OnReloadFinished(const USkeletalMeshComponent* Mesh)
{
	if(!CheckIsPlayer(Mesh))
		return;
	InReloadProgress = false;
	CurrentWeapon->HideMag();
	DestroyMag();
}

void UWeaponComponent::OnChangeMag(const USkeletalMeshComponent* Mesh)
{
	if(!CheckIsPlayer(Mesh))
		return;
	CurrentWeapon->UnHideMag();
	PlaySound(ChangeMagSound);
	SpawnMag();
}

void UWeaponComponent::SpawnMag()
{
	if(!GetWorld() && !CurrentWeapon)
		return;

	ABaseCharacter* Character = Cast<ABaseCharacter>(GetOwner());
	if(!Character)
		return;
	CurrentMag = GetWorld()->SpawnActor<AMag>(CurrentWeapon->GetMagClass());
	if(!CurrentWeapon)
		return;
	CurrentWeapon->SetOwner(Character);
	AttachActorToSocket(CurrentMag, Character->GetMesh(), CurrentWeapon->GetMagSocketName());
}

void UWeaponComponent::DestroyMag()
{
	if(!CurrentMag)
		return;
	CurrentMag->Destroy();
	CurrentMag = nullptr;
}

void UWeaponComponent::OnEquipFinished(const USkeletalMeshComponent* Mesh)
{
	if(!CheckIsPlayer(Mesh))
		return;
	InEquipProgress = false;
}

void UWeaponComponent::OnChangeWeapon(const USkeletalMeshComponent* Mesh)
{
	if(!CheckIsPlayer(Mesh))
		return;
	PlaySound(ChangeWeaponSound);
	EquipWeapon(CurrentWeaponIndex);
}

