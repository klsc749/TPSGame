// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/FindEnemyService.h"

#include "AIController.h"
#include "AI/TPSAIPerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

UFindEnemyService::UFindEnemyService()
{
	NodeName = "Find Enemy";
}

void UFindEnemyService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	const auto Controller = OwnerComp.GetAIOwner();
	const auto Blackboard = OwnerComp.GetBlackboardComponent();
	if(Controller && Blackboard)
	{
		const auto AIPerceptionComponent = Cast<UTPSAIPerceptionComponent>(Controller->GetComponentByClass(UTPSAIPerceptionComponent::StaticClass()));
		if(AIPerceptionComponent)
		{
			Blackboard->SetValueAsObject(EnemyActorKey.SelectedKeyName, AIPerceptionComponent->GetClosestEnemy());
		}
	}
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
