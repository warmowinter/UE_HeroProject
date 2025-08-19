// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "items/Weapon_ItemActor.h"
#include "PistolWeapon_ItemActor.generated.h"

/**
 * 
 */
UCLASS()
class AIMTOSHOOT_API APistolWeapon_ItemActor : public AWeapon_ItemActor
{
	GENERATED_BODY()
public:
	APistolWeapon_ItemActor();

	void InitType() override;
};
