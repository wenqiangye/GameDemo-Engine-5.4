// Fill out your copyright notice in the Description page of Project Settings.

#include "GameBaseActor.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "TimerManager.h"

DEFINE_LOG_CATEGORY_STATIC(LogGameBaseGeometry, All, All)

// Sets default values
AGameBaseActor::AGameBaseActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to
	// improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	SetRootComponent(BaseMesh);
}

// Called when the game starts or when spawned
void AGameBaseActor::BeginPlay()
{
	Super::BeginPlay();

	InitialLocation = GetActorLocation();

	SetColor(GeometryData.Color);

	GetWorldTimerManager().SetTimer(TimerHandle, this, &AGameBaseActor::OnTimerFired, GeometryData.TimeRate, true);

	// PrintTypes();
}

// Called every frame
void AGameBaseActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	HandleMovement();
}

void AGameBaseActor::PrintTypes()
{
	UE_LOG(LogGameBaseGeometry, Warning, TEXT("Actor name: %s"), *GetName());
	UE_LOG(LogGameBaseGeometry, Warning, TEXT("Weapons num: %d,kills  num: %i"), WeaponsNum, KillsNum);
	UE_LOG(LogGameBaseGeometry, Warning, TEXT("Health: %f"), Health);
	UE_LOG(LogGameBaseGeometry, Warning, TEXT("IsDead: %d"), IsDead);
	UE_LOG(LogGameBaseGeometry, Warning, TEXT("HasWeapon: %d"), static_cast<int>(HasWeapon));
}

void AGameBaseActor::HandleMovement()
{
	switch (GeometryData.MoveType)
	{
	case EMovementType::Sin:
		{
			// z = z0 + amplitude*sin(freq*t)
			FVector CurrentLocation = GetActorLocation();
			if (GetWorld())
			{
				float time = GetWorld()->GetTimeSeconds();
				CurrentLocation.Z =
					InitialLocation.Z + GeometryData.Amplitude * FMath::Sin(GeometryData.Frequency * time);
				SetActorLocation(CurrentLocation);
			}
		}
		break;
	case EMovementType::Static:
		break;
	default:
		break;
	}
}

void AGameBaseActor::SetColor(const FLinearColor &Color)
{
	if (!BaseMesh) return;
	UMaterialInstanceDynamic *DynMaterial = BaseMesh->CreateDynamicMaterialInstance(0);
	if (DynMaterial)
	{
		DynMaterial->SetVectorParameterValue("Color", Color);
	}
}

void AGameBaseActor::OnTimerFired()
{
	if (++TimerCount <= MaxTimerCount)
	{
		const FLinearColor NewColor = FLinearColor::MakeRandomColor();
		UE_LOG(LogGameBaseGeometry, Display, TEXT("TimerCount: %d, Color to set up: %s"), TimerCount,
			   *NewColor.ToString());
		SetColor(NewColor);
	}
	else
	{
		UE_LOG(LogGameBaseGeometry, Warning, TEXT("Timer has been stopped!"));
		GetWorldTimerManager().ClearTimer(TimerHandle);
	}
}
