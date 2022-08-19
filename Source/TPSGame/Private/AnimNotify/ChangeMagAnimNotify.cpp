// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/ChangeMagAnimNotify.h"

void UChangeMagAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	OnNotified.Broadcast(MeshComp);
	Super::Notify(MeshComp, Animation);
}