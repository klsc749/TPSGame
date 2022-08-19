// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Mag.generated.h"

UCLASS()
class TPSGAME_API AMag : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMag();

protected:
	UPROPERTY(EditDefaultsOnly, Category="Mesh")
	UStaticMeshComponent* MagMesh;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:	
};
