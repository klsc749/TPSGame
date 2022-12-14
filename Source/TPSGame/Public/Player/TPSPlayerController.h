// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TPSPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class TPSGAME_API ATPSPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	UFUNCTION()
	void SetHealth(float HealthPercent);
	UPROPERTY()
	class ABaseGameHUD* GameHUD;
protected:
	virtual void BeginPlay() override;
private:
};
