// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GDPlayerHUDWidget.h"
#include "Components/GDHealthyComponent.h"

float UGDPlayerHUDWidget::GetHealthPercent() const
{
	const auto Player = GetOwningPlayerPawn();
	if (!Player) return 0.0f;

	const auto Component = Player->GetComponentByClass(UGDHealthyComponent::StaticClass());
	const auto HealthComponent = Cast<UGDHealthyComponent>(Component);

	if (!HealthComponent) return 0.0f;

	return HealthComponent->GetHealthyPercent();
}
