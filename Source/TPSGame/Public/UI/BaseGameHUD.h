// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerHudWidget.h"
#include "GameFramework/HUD.h"
#include "BaseGameHUD.generated.h"

/**
 * 
 */
UCLASS()
class TPSGAME_API ABaseGameHUD : public AHUD
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly, Category="Widget")
	TSubclassOf<UUserWidget> HUDWidgetClass;
	
public:
	UPROPERTY()
	UPlayerHudWidget* PlayerHudWidget;
	virtual void DrawHUD() override;
	virtual void BeginPlay() override;
private:
	void DrawCrossHair();	
};
