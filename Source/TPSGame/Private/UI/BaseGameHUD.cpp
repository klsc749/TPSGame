// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BaseGameHUD.h"

#include "Blueprint/UserWidget.h"
#include "Engine/Canvas.h"


void ABaseGameHUD::DrawHUD()
{
	Super::DrawHUD();
	DrawCrossHair();
}

void ABaseGameHUD::BeginPlay()
{
	Super::BeginPlay();
	if(!GetWorld())
		return;
	PlayerHudWidget = CreateWidget<UPlayerHudWidget>(GetWorld(), HUDWidgetClass);
	PlayerHudWidget->AddToViewport();
}

void ABaseGameHUD::DrawCrossHair()
{
	// get screen center
	const TInterval<float> Center(Canvas->SizeX * 0.5f, Canvas->SizeY * 0.5f);
	const float HalfLineSize = 10.0f;
	const float LineThickness = 2.0f;
	const FLinearColor LineColor = FLinearColor::Green;
	
	DrawLine(Center.Min - HalfLineSize, Center.Max, Center.Min + HalfLineSize, Center.Max, LineColor, LineThickness);
	DrawLine(Center.Min, Center.Max  - HalfLineSize, Center.Min, Center.Max + HalfLineSize, LineColor, LineThickness);
}
