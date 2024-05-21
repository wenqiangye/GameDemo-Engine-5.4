// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/GDHealthyComponent.h"
#include "GameFramework/Actor.h"


DEFINE_LOG_CATEGORY_STATIC(LogGDHealthComponent, All, All)

// Sets default values for this component's properties
UGDHealthyComponent::UGDHealthyComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UGDHealthyComponent::BeginPlay()
{
	Super::BeginPlay();
	SetHealth(MaxHealth);

	check(MaxHealth>0);

	
	AActor* ComponentOwner = GetOwner();
	if (ComponentOwner)
	{
		ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &UGDHealthyComponent::OnTakeAnyDamage);
	}
	// ...
}

void UGDHealthyComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
                                          AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0.0f || IsDead() || !GetWorld()) return;
	// 防止出边界
	SetHealth(Health - Damage);

	GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);

	if (IsDead())
	{
		OnDeath.Broadcast();
	}
	else if (AutoHeal)
	{
		GetWorld()->GetTimerManager().SetTimer(HealTimerHandle, this, &UGDHealthyComponent::HealUpdate, HealUpdateTime,
		                                       true, HealDelay);
	}
}

void UGDHealthyComponent::HealUpdate()
{
	SetHealth(Health + HealModifier);
	OnHealthChanged.Broadcast(Health);

	if (FMath::IsNearlyEqual(Health, MaxHealth) && GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);
	}
}

void UGDHealthyComponent::SetHealth(float NewHealth)
{
	Health = FMath::Clamp(NewHealth, 0.0f, MaxHealth);
	OnHealthChanged.Broadcast(Health);
}
