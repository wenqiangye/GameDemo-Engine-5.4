// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/GMCharacterMovementComponent.h"
#include "Player/GameDemoCharacter.h"

float UGMCharacterMovementComponent::GetMaxSpeed() const
{
	const float MaxSpeed = Super::GetMaxSpeed();
	AGameDemoCharacter* Player = Cast<AGameDemoCharacter>(GetPawnOwner());

	return Player && Player->IsRunining() ? MaxSpeed * RunModifier : MaxSpeed;
}
