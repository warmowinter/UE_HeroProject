// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "RifleWeapon.generated.h"

/**
 * 
 */
UCLASS()
class AIMTOSHOOT_API ARifleWeapon : public AWeaponBase
{
	GENERATED_BODY()
public:
	ARifleWeapon();

	void WeaponFiring() override;

};
