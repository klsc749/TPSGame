// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/NextLocationTask.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"


UNextLocationTask::UNextLocationTask()
{
	NodeName = "NextLocation";
}

EBTNodeResult::Type UNextLocationTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const auto Controller = OwnerComp.GetAIOwner();
	const auto Blackboard = OwnerComp.GetBlackboardComponent();
	if(!Controller || !Blackboard)
	{
		return EBTNodeResult::Failed;
	}
	const auto Pawn = Controller->GetPawn();
	if(!Pawn)
	{
		return EBTNodeResult::Failed;
	}
	const auto NavSys = UNavigationSystemV1::GetCurrent(Pawn);
	if(!NavSys)
	{
		return EBTNodeResult::Failed;
	}
	FNavLocation NavLocation;
	auto Location = Pawn->GetActorLocation();
	if(!SelfCenter)
	{
		const auto CenterActor = Cast<AActor>(Blackboard->GetValueAsObject(CenterActorKey.SelectedKeyName));
		if(!CenterActor)
		{
			Location = CenterActor->GetActorLocation();
		}
	}
	const auto Found = NavSys->GetRandomReachablePointInRadius(Location, Radius, NavLocation);
	if(!Found)
	{
		return EBTNodeResult::Failed;
	}
	Blackboard->SetValueAsVector(AimLocationKey.SelectedKeyName, NavLocation.Location);
	return EBTNodeResult::Succeeded;
}
