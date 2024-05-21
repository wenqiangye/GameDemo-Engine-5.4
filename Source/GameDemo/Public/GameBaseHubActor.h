// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameBaseActor.h"
#include "GameFramework/Actor.h"
#include "GameBaseHubActor.generated.h"

USTRUCT()
struct FGameBasePayload
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	TSubclassOf<AGameBaseActor> GameBaseClass;

	UPROPERTY(EditAnywhere)
	FGeometryData GeometryData;

	UPROPERTY(EditAnywhere)
	FTransform InitialTransform;
};

UCLASS()
class GAMEDEMO_API AGameBaseHubActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGameBaseHubActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AGameBaseActor> GameBaseClass;

	UPROPERTY(EditAnywhere)
	TArray<FGameBasePayload> GameBasePayloads;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	void DoActorSpawn1();
	void DoActorSpawn2();
	void DoActorSpawn3();
};
