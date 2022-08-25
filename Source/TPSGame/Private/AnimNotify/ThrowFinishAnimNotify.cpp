// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/ThrowFinishAnimNotify.h"

void UThrowFinishAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	OnNotified.Broadcast(MeshComp);
	Super::Notify(MeshComp, Animation);
}
