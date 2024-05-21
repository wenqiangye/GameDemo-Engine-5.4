// Fill out your copyright notice in the Description page of Project Settings.

#include "GameBaseHubActor.h"

#include "GameBaseActor.h"

// Sets default values
AGameBaseHubActor::AGameBaseHubActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to
	// improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AGameBaseHubActor::BeginPlay() { Super::BeginPlay(); }

// Called every frame
void AGameBaseHubActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	DoActorSpawn1();
	DoActorSpawn2();
	DoActorSpawn3();
}

void AGameBaseHubActor::DoActorSpawn1()
{
	UWorld *World = GetWorld();
	if (World)
	{
		for (int32 i = 0; i < 10; ++i)
		{
			const FTransform GameBaseActorTransform =
				FTransform(FRotator::ZeroRotator, FVector(0.0f, 300.0f * i, 300.0f));
			AGameBaseActor *GameBaseActor = World->SpawnActor<AGameBaseActor>(GameBaseClass, GameBaseActorTransform);
			if (GameBaseActor)
			{
				FGeometryData Data;
				Data.MoveType = FMath::RandBool() ? EMovementType::Static : EMovementType::Sin;
				GameBaseActor->SetGameBaseData(Data);
			}
		}
	}
}

void AGameBaseHubActor::DoActorSpawn2()
{
	UWorld *World = GetWorld();
	if (World)
	{
		for (int32 i = 0; i < 10; ++i)
		{
			const FTransform GameBaseActorTransform =
				FTransform(FRotator::ZeroRotator, FVector(0.0f, 300.0f * i, 700.0f));
			AGameBaseActor *GameBaseActor =
				World->SpawnActorDeferred<AGameBaseActor>(GameBaseClass, GameBaseActorTransform);
			if (GameBaseActor)
			{
				FGeometryData Data;
				Data.MoveType = FMath::RandBool() ? EMovementType::Static : EMovementType::Sin;
				Data.Color = FLinearColor::MakeRandomColor();
				GameBaseActor->SetGameBaseData(Data);
				GameBaseActor->FinishSpawning(GameBaseActorTransform);
			}
		}
	}
}

void AGameBaseHubActor::DoActorSpawn3()
{
	UWorld *World = GetWorld();
	if (World)
	{
		for (const FGameBasePayload Payload : GameBasePayloads)
		{
			AGameBaseActor *GameBaseActor =
				World->SpawnActorDeferred<AGameBaseActor>(Payload.GameBaseClass, Payload.InitialTransform);
			if (GameBaseActor)
			{
				GameBaseActor->SetGameBaseData(Payload.GeometryData);
				GameBaseActor->FinishSpawning(Payload.InitialTransform);
			}
		}
	}
}
