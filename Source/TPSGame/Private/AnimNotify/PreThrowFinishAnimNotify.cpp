// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/PreThrowFinishAnimNotify.h"

void UPreThrowFinishAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	OnNotified.Broadcast(MeshComp);
	Super::Notify(MeshComp, Animation);
}