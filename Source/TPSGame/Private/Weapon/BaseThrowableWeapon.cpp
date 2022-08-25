// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/BaseThrowableWeapon.h"

// Sets default values
ABaseThrowableWeapon::ABaseThrowableWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>("Mesh");
	SetRootComponent(Mesh);
}

// Called when the game starts or when spawned
void ABaseThrowableWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}


