// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/TPSAIPerceptionComponent.h"

#include "AIController.h"
#include "HealthComponent/HealthComponent.h"
#include "Perception/AISense_Sight.h"

AActor* UTPSAIPerceptionComponent::GetClosestEnemy() const
{
	TArray<AActor*> PercieveActors;
	GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), PercieveActors);
	if(PercieveActors.Num() == 0)
	{
		return nullptr;
	}

	const auto AIController = Cast<AAIController>(GetOwner());
	if(!AIController)
	{
		return nullptr;
	}

	const auto Pawn = AIController->GetPawn();
	if(!Pawn)
		return  nullptr;
	
	float BestDistance = MAX_FLT;
	AActor* BestPawn = nullptr;
	for(const auto PerceieveActor : PercieveActors)
	{
		const auto HealthComponent = Cast<UHealthComponent>(PerceieveActor->GetComponentByClass(UHealthComponent::StaticClass()));
		if(HealthComponent && !HealthComponent->IsDead())
		{
			const auto CurrentDistance = (PerceieveActor->GetActorLocation() - Pawn->GetActorLocation()).Size();
			if(CurrentDistance < BestDistance)
			{
				BestPawn = PerceieveActor;
				BestDistance = CurrentDistance;
			}
		}
	}

	return BestPawn;
}
