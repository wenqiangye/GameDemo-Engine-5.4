// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/GDBaseWeapon.h"
#include "GDSwordWeapon.generated.h"

/**
 * 
 */
UCLASS()
class GAMEDEMO_API AGDSwordWeapon : public AGDBaseWeapon
{
	GENERATED_BODY()

public:
	virtual void StartFire() override;
protected:
	void MakeAttack();
};
