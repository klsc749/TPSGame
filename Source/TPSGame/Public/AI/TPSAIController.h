// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TPSAIPerceptionComponent.h"
#include "TPSAIController.generated.h"

/**
 * 
 */
UCLASS()
class TPSGAME_API ATPSAIController : public AAIController
{
	GENERATED_BODY()
public:
	ATPSAIController();
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components")
	UTPSAIPerceptionComponent* TPSPerceptionComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components")
	FName FocusOnKeyName = "EnemyActor";
	virtual void OnPossess(APawn* InPawn) override;
	virtual void Tick(float DeltaSeconds) override;
private:
	AActor* GetFocusOnActor();
};
