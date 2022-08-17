// Fill out your copyright notice in the Description page of Project Settings.


#include "Dev/DevDamageActor.h"

#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ADevDamageActor::ADevDamageActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SceneComponent = CreateDefaultSubobject<USceneComponent>("RootComponent");
	SetRootComponent(SceneComponent);
}

// Called when the game starts or when spawned
void ADevDamageActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADevDamageActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UWorld* World = GetWorld();
	if(World)
	{
		DrawDebugSphere(World, GetActorLocation(), Radius, 24, Color);
		UGameplayStatics::ApplyRadialDamage(World, Damage, GetActorLocation(), Radius, nullptr, {}, this, nullptr, DoFullDamage);
	}
}

