// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/GDMonsterPawn.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/GDHealthyComponent.h"


// Sets default values
AGDMonsterPawn::AGDMonsterPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(GetRootComponent());

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);

	HealthComponent = CreateDefaultSubobject<UGDHealthyComponent>("HealthComponent");

	HealthTextComponent = CreateDefaultSubobject<UTextRenderComponent>("HealthTextComponent");
	HealthTextComponent->SetupAttachment(GetRootComponent());
	HealthTextComponent->SetOwnerNoSee(true);
}

// Called when the game starts or when spawned
void AGDMonsterPawn::BeginPlay()
{
	Super::BeginPlay();

	OnHealthChanged((HealthComponent->GetHealth()));
	HealthComponent->OnDeath.AddUObject(this, &AGDMonsterPawn::OnDeath);
	HealthComponent->OnHealthChanged.AddUObject(this, &AGDMonsterPawn::OnHealthChanged);
}

// Called every frame
void AGDMonsterPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AGDMonsterPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AGDMonsterPawn::OnDeath()
{
}

void AGDMonsterPawn::OnHealthChanged(float Health)
{
	HealthTextComponent->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), Health)));
}
