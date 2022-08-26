// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponComponent/WeaponComponent.h"

#include "DrawDebugHelpers.h"
#include "AnimNotify/ChangeMagAnimNotify.h"
#include "AnimNotify/ChangeWeaponAnimNotify.h"
#include "AnimNotify/EquipFinishAnimNotify.h"
#include "AnimNotify/ReloadFinishAnimNotify.h"
#include "AnimNotify/PreThrowFinishAnimNotify.h"
#include "AnimNotify/ThrowFinishAnimNotify.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/GameplayStaticsTypes.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/BaseCharacter.h"
#include "Projectile/BaseProjectile.h"

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
	const APawn* Player = Cast<APawn>(GetOwner());
	if(!Player)
		return;
	if (Player->HasAuthority())
	{
		StartFireMulticast();
	}
	else
	{
		StartFireOnServer();
	}
}

void UWeaponComponent::StopFire()
{
	const APawn* Player = Cast<APawn>(GetOwner());
	if(!Player)
		return;
	if (Player->HasAuthority())
	{
		EndFireMulticast();
	}
	else
	{
		EndFireOnServer();
	}
}

void UWeaponComponent::StartFireOnServer_Implementation()
{
	StartFireMulticast();
}

void UWeaponComponent::StartFireMulticast_Implementation()
{
	if(!CanFire())
		return;
	if(!CurrentWeapon)
		return;
	CurrentWeapon->StartFire();
}

void UWeaponComponent::EndFireOnServer_Implementation()
{
	EndFireMulticast();
}

void UWeaponComponent::EndFireMulticast_Implementation()
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
	const APawn* Player = Cast<APawn>(GetOwner());
	if(!Player)
		return;
	if (Player->HasAuthority())
	{
		ChangeWeaponMulticast();
	}
	else
	{
		ChangeWeaponOnServer();
	}
}

void UWeaponComponent::ChangeWeaponOnServer_Implementation()
{
	ChangeWeaponMulticast();
}

void UWeaponComponent::ChangeWeaponMulticast_Implementation()
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
	const APawn* Player = Cast<APawn>(GetOwner());
	if(!Player)
		return;
	if (Player->HasAuthority())
	{
		ReloadMultiCast();
	}
	else
	{
		ReloadOnServer();
	}
}

void UWeaponComponent::ReloadOnServer_Implementation()
{
	ReloadMultiCast();
}

void UWeaponComponent::ReloadMultiCast_Implementation()
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

void UWeaponComponent::BeginThrow()
{
	if(!CanThrow())
		return;
	InThrowProgress = true;
	PlayAnimationMontage(PreThrow);
}

void UWeaponComponent::FinishThrow()
{
	if(!InThrowProgress)
		return;
	PlayAnimationMontage(ThrowMontage);
	GetOwner()->GetWorldTimerManager().ClearTimer(TraceTimerHandle);
	SpawnGrenade();
}

void UWeaponComponent::OnFinishPreThrow(const USkeletalMeshComponent* Mesh)
{
	if(!CheckIsPlayer(Mesh))
		return;
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if(!Character)
		return;
	Character->PlayAnimMontage(AimIdle, 1.0f, "1");
	GetOwner()->GetWorldTimerManager().SetTimer(TraceTimerHandle, this, &UWeaponComponent::MakeTrace, GetWorld()->DeltaTimeSeconds, true);
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
	ABaseCharacter* PLayer = Cast<ABaseCharacter>(GetOwner());
	if(!PLayer)
		return;
	UAudioComponent* AudioComponent = PLayer->AudioComponent;
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
			ReloadFinishAnimNotify->OnNotified.AddUObject(this, &UWeaponComponent::OnReloadFinished);
			break;
		}
	}

	for(const FAnimNotifyEvent& NotifyEvent : Notifies)
	{
		UChangeMagAnimNotify* ChangeMagAnimNotify = Cast<UChangeMagAnimNotify>(NotifyEvent.Notify);
		if(ChangeMagAnimNotify)
		{
			ChangeMagAnimNotify->OnNotified.AddUObject(this, &UWeaponComponent::OnChangeMag);
			break;
		}
	}
	if(!PreThrow)
		return;
	Notifies = PreThrow->Notifies;
	for(const FAnimNotifyEvent& NotifyEvent : Notifies)
	{
		UPreThrowFinishAnimNotify* PreThrowAnimNotify = Cast<UPreThrowFinishAnimNotify>(NotifyEvent.Notify);
		if(PreThrowAnimNotify)
		{
			PreThrowAnimNotify->OnNotified.AddUObject(this, &UWeaponComponent::OnFinishPreThrow);
			break;
		}
	}
	if(!ThrowMontage)
		return;
	Notifies = ThrowMontage->Notifies;
	for(const FAnimNotifyEvent& NotifyEvent : Notifies)
	{
		UThrowFinishAnimNotify* ThrowFinishAnimNotify = Cast<UThrowFinishAnimNotify>(NotifyEvent.Notify);
		if(ThrowFinishAnimNotify)
		{
			ThrowFinishAnimNotify->OnNotified.AddUObject(this, &UWeaponComponent::OnFinishThrow);
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
	return !Player->IsSprinting() && !InEquipProgress && !InReloadProgress && !InThrowProgress && !CurrentWeapon->IsMagEmpty();
}

bool UWeaponComponent::CanReload() const
{
	return !InEquipProgress && !InReloadProgress && !InThrowProgress;
}

bool UWeaponComponent::CanThrow() const
{
	return  !InEquipProgress  && !InReloadProgress && !InThrowProgress;
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

void UWeaponComponent::OnFinishThrow(const USkeletalMeshComponent* Mesh)
{
	if(!CheckIsPlayer(Mesh))
		return;
	InThrowProgress = false;
}


void UWeaponComponent::MakeTrace()
{
	if(!GetWorld())
		return;
	const ABaseCharacter* Player = Cast<ABaseCharacter>(GetOwner());
	if(!Player)
		return;
	FVector StartLocation;
	FRotator SocketRotator;
	Player->GetMesh()->GetSocketWorldLocationAndRotation(ThrowSocketName, StartLocation, SocketRotator);
	FPredictProjectilePathParams PredictProjectilePathParams;
	PredictProjectilePathParams.StartLocation = StartLocation;
	FVector Offset(0, 0, 0.5f);
	PredictProjectilePathParams.LaunchVelocity = (UKismetMathLibrary::GetForwardVector(Player->GetControlRotation()) + Offset) * ThrowSpeed;
	TArray<AActor*> ActorToIgnore;
	ActorToIgnore.Add(GetOwner());
	PredictProjectilePathParams.ActorsToIgnore = ActorToIgnore;
	PredictProjectilePathParams.OverrideGravityZ = 1.0f;
	FPredictProjectilePathResult ProjectilePathResult;
	
	UGameplayStatics::PredictProjectilePath(GetWorld(), PredictProjectilePathParams, ProjectilePathResult);

	for (const auto point : ProjectilePathResult.PathData)
	{
		DrawDebugPoint(GetWorld(), point.Location, 5.0f, FColor::Green, false, GetWorld()->DeltaTimeSeconds);
	}
	/*int PointNum = ProjectilePathResult.PathData.Num();
	for(int i = 0; i <  PointNum; i++)
	{
		SplineComponent->AddSplinePointAtIndex(ProjectilePathResult.PathData[i].Location, i, ESplineCoordinateSpace::Local);
	}
	for(int i = 0; i < PointNum - 1; i++)
	{
		USplineMeshComponent* SplineMeshComponent = NewObject<USplineMeshComponent>(GetOwner(), USplineMeshComponent::StaticClass());
		SplineMeshComponent->SetStaticMesh(SplineMesh);
		const FVector StartPoint = SplineComponent->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::Local);
		const FVector EndPoint = SplineComponent->GetLocationAtSplinePoint(i + 1, ESplineCoordinateSpace::Local);
		const FVector StartTangent = SplineComponent->GetTangentAtSplinePoint(i, ESplineCoordinateSpace::Local);
		const FVector EndTangent = SplineComponent->GetTangentAtSplinePoint(i + 1, ESplineCoordinateSpace::Local);

		SplineMeshComponent->SetStartAndEnd(StartPoint, StartTangent, EndPoint, EndTangent);
		SplineMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}*/
}

void UWeaponComponent::SpawnGrenade()
{
	if(!GetWorld())
		return;
	ABaseCharacter* Player = Cast<ABaseCharacter>(GetOwner());
	if(!Player)
		return;
	auto Projectile = GetWorld()->SpawnActorDeferred<ABaseProjectile>(ProjectileClass, Player->GetMesh()->GetSocketTransform(ThrowSocketName));
	if(!Projectile)
		return;
	//set projectile params
	Projectile->SetOwner(GetOwner());
	const FVector Offset(0, 0, 0.5f);
	Projectile->SetShotDirection(UKismetMathLibrary::GetForwardVector(Player->GetControlRotation()) + Offset);
	DrawDebugLine(GetWorld(), Player->GetMesh()->GetSocketTransform(ThrowSocketName).GetLocation(),
		Player->GetMesh()->GetSocketTransform(ThrowSocketName).GetLocation() + 600 * (UKismetMathLibrary::GetForwardVector(Player->GetControlRotation()) + Offset), FColor::Red, false, 3.0);
	UGameplayStatics::FinishSpawningActor(Projectile, Player->GetMesh()->GetSocketTransform(ThrowSocketName));
}

