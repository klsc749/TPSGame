// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/TPSAIController.h"

#include "AI/AICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

ATPSAIController::ATPSAIController()
{
	TPSPerceptionComponent = CreateDefaultSubobject<UTPSAIPerceptionComponent>("PerceptionComponent");
	SetPerceptionComponent(*TPSPerceptionComponent);
}


void ATPSAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	const auto AICharacter = Cast<AAICharacter>(InPawn);
	if(!AICharacter)
		return;
	RunBehaviorTree(AICharacter->BehaviorTree);
}

void ATPSAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	const auto AimActor = GetFocusOnActor();
	SetFocus(AimActor);
}

AActor* ATPSAIController::GetFocusOnActor()
{
	if(!GetBlackboardComponent())
		return nullptr;
	return Cast<AActor>(GetBlackboardComponent()->GetValueAsObject(FocusOnKeyName));
}
