// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DevDamageActor.generated.h"

UCLASS()
class TPSGAME_API ADevDamageActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADevDamageActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* SceneComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Radius = 300;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FColor Color = FColor::Blue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool DoFullDamage = false;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
