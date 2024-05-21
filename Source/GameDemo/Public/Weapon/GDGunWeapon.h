// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/GDBaseWeapon.h"
#include "GDGunWeapon.generated.h"

/**
 * 
 */
UCLASS()
class GAMEDEMO_API AGDGunWeapon : public AGDBaseWeapon
{
	GENERATED_BODY()

public:
	virtual void StartFire() override;
	virtual void StopFire() override;

protected:
	virtual void MakeShot() override;
	virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float TimeBetweenShot = 0.1f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float BulletSpread = 1.5f;

private:
	FTimerHandle ShotTimerHandler;
};
