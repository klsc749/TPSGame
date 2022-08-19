// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/ChangeWeaponAnimNotify.h"

void UChangeWeaponAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	OnNotified.Broadcast(MeshComp);
	Super::Notify(MeshComp, Animation);
}