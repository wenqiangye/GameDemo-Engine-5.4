// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GDMonsterPawn.generated.h"

class UTextRenderComponent;
class UCameraComponent;
class USpringArmComponent;
class UGDHealthyComponent;

UCLASS()
class GAMEDEMO_API AGDMonsterPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AGDMonsterPawn();

protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Component")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Component")
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Component")
	UGDHealthyComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Component")
	UTextRenderComponent* HealthTextComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* DeathAnimeMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float LifeSpanDeath = 5.0f;

	virtual void BeginPlay() override;
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void OnDeath();
	void OnHealthChanged(float Health);
};
