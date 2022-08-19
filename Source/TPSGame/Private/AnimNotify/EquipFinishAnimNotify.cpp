// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/EquipFinishAnimNotify.h"

void UEquipFinishAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	OnNotified.Broadcast(MeshComp);
	Super::Notify(MeshComp, Animation);
}
