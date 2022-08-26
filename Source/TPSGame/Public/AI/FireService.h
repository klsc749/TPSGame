// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "FireService.generated.h"

/**
 * 
 */
UCLASS()
class TPSGAME_API UFireService : public UBTService
{
	GENERATED_BODY()
public:
	UFireService();
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components")
	FName FocusOnKeyName = "EnemyActor";
};
