// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/BaseProjectile.h"

#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnitConversion.h"

// Sets default values
ABaseProjectile::ABaseProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>("CollisionComponent");
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComponent->SetCollisionResponseToChannels(ECollisionResponse::ECR_Block);
	CollisionComponent->IgnoreActorWhenMoving(GetOwner(), true);
	CollisionComponent->InitSphereRadius(5.0f);
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");
	ProjectileMovementComponent->InitialSpeed = 2000.f;
	ProjectileMovementComponent->ProjectileGravityScale = 0;
	VfxComponent = CreateDefaultSubobject<UWeaponVFXComponent>("WeaponVFXComponent");
	SetRootComponent(CollisionComponent);
}


// Called when the game starts or when spawned
void ABaseProjectile::BeginPlay()
{
	Super::BeginPlay();
	check(CollisionComponent);
	check(ProjectileMovementComponent);

	ProjectileMovementComponent->Velocity = ShotDirection * ProjectileMovementComponent->InitialSpeed;
	CollisionComponent->OnComponentHit.AddDynamic(this, &ABaseProjectile::OnProjectileHit);
	SetLifeSpan(5.0f);
}

void ABaseProjectile::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if(!GetWorld())
		return;
	ProjectileMovementComponent->StopMovementImmediately();
	UGameplayStatics::ApplyRadialDamage(GetWorld(), DamageAmount, GetActorLocation(), DamageRadius, UDamageType::StaticClass(),
		{GetOwner()}, this, GetController(), DoFullDamage);
	VfxComponent->PlayFXImpact(Hit);

	// Add radial force

	TArray<FHitResult> OutHits;
	FCollisionShape ColSphere = FCollisionShape::MakeSphere(DamageRadius);
	bool IsHit = GetWorld()->SweepMultiByChannel(OutHits, GetActorLocation(), GetActorLocation(), FQuat::Identity, ECC_WorldStatic, ColSphere);

	if(IsHit)
	{
		for(auto& OutHit : OutHits)
		{
			UStaticMeshComponent* MeshComp = Cast<UStaticMeshComponent>((Hit.GetActor())->GetRootComponent());
			if (MeshComp)
			{
				MeshComp->SetSimulatePhysics(true);
				MeshComp->AddRadialImpulse(GetActorLocation(), DamageRadius, Force, ERadialImpulseFalloff::RIF_Constant, true);
			}
		}
	}
	
	Destroy();
}

AController* ABaseProjectile::GetController() const
{
	const auto Pawn = Cast<APawn>(GetOwner());
	return Pawn ? Pawn->GetController() : nullptr;
}


