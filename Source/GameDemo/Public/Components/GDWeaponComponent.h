// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GDWeaponComponent.generated.h"

class AGDBaseWeapon;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAMEDEMO_API UGDWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UGDWeaponComponent();

	void StartFire();
	void StopFire();
	void NextWeapon();

	UFUNCTION(BlueprintCallable, Category="Weapon")
	FString GetWeaponType() const;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	TArray<TSubclassOf<AGDBaseWeapon>> WeaponClasses;

	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	FName WeaponEquipSocketName = "WeaponSocket";

	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	FName WeaponArmorySocketName = "ArmorySocket";

private:
	UPROPERTY()
	AGDBaseWeapon* CurrentWeapon = nullptr;

	UPROPERTY()
	TArray<AGDBaseWeapon*> Weapons;

	int32 CurrentWeaponIndex = 0;


	void SpawnWeapons();
	void AttachWeaponToSocket(AGDBaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName);

	void EquipWeapon(int32 WeaponIndex);
};
