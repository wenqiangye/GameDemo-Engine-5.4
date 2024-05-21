// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GDDamageActor.generated.h"


UCLASS()
class GAMEDEMO_API AGDDamageActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGDDamageActor();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Component")
	USceneComponent* SceneComponent;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float Radius = 300.0f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FColor SphereColor = FColor::Red;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float Damage = 10.0f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool DoFullDamage = false;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSubclassOf<UDamageType> DamageType;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
