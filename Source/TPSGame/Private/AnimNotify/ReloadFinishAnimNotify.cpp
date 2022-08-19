// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/ReloadFinishAnimNotify.h"

void UReloadFinishAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	OnNotified.Broadcast(MeshComp);
	Super::Notify(MeshComp, Animation);
}

