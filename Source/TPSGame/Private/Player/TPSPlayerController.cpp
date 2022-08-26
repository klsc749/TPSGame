// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/TPSPlayerController.h"

#include "Components/ProgressBar.h"
#include "UI/BaseGameHUD.h"

void ATPSPlayerController::SetHealth(float HealthPercent)
{
	GameHUD = GameHUD == nullptr ? Cast<ABaseGameHUD>(GetHUD()) : GameHUD;
	if(GameHUD && GameHUD->PlayerHudWidget && GameHUD->PlayerHudWidget->HealthBar)
	{
		UE_LOG(LogTemp, Warning, TEXT("SetPersent"));
		GameHUD->PlayerHudWidget->HealthBar->SetPercent(HealthPercent);
	}
}

void ATPSPlayerController::BeginPlay()
{
	Super::BeginPlay();
	GameHUD = Cast<ABaseGameHUD>(GetHUD());
}
