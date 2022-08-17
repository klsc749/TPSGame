// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponComponent/WeaponComponent.h"

#include "Player/BaseCharacter.h"

// Sets default values for this component's properties
UWeaponComponent::UWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	CurrentWeaponIndex = 0;
	SpawnWeapon();
	EquipWeapon(CurrentWeaponIndex);
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
		Weapons.Add(Weapon);
		AttachWeaponToSocket(Weapon, Character->GetMesh(), Weapon->GetArmorySocketName());
	}
}

void UWeaponComponent::StartFire()
{
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
		AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), CurrentWeapon->GetArmorySocketName());
	}
	
	CurrentWeapon = Weapons[WeaponIndex];
	AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), CurrentWeapon->GetEquipSocketName());
	PlayAnimationMontage(EquipMontage);
}

void UWeaponComponent::NextWeapon()
{
	if(Weapons.Num() == 0)
		return;
	CurrentWeaponIndex = (CurrentWeaponIndex + 1) % Weapons.Num();
	EquipWeapon(CurrentWeaponIndex);
}

void UWeaponComponent::AttachWeaponToSocket(ABaseWeapon* Weapon, USceneComponent* SceneComponent,
	const FName& SocketName) const
{
	if(!Weapon || !SceneComponent)
		return;
	const FAttachmentTransformRules AttachmentTransformRules(EAttachmentRule::SnapToTarget, false);
	Weapon->AttachToComponent(SceneComponent, AttachmentTransformRules, SocketName);
}

void UWeaponComponent::PlayAnimationMontage(UAnimMontage* Animation)
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if(!Character)
		return;
	Character->PlayAnimMontage(Animation);
}

