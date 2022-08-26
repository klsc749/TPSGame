// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/FireService.h"

#include "AIController.h"
#include "AI/AICharacter.h"
#include "AI/AIWeaponComponent.h"

UFireService::UFireService()
{
	NodeName = "Fire Service";
}

void UFireService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	const auto Controller = OwnerComp.GetAIOwner();
	const auto Blackboard = OwnerComp.GetBlackboardComponent();
	
	if(Controller && Blackboard)
	{
		AAICharacter* AICharacter = Cast<AAICharacter>(Controller->GetPawn());
		if(!AICharacter)
			return;
		const auto UaiWeaponComponent = Cast<UAIWeaponComponent>(AICharacter->GetComponentByClass(UAIWeaponComponent::StaticClass()));
		if(UaiWeaponComponent)
		{
			UaiWeaponComponent->StartFire();
		}
	}
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
