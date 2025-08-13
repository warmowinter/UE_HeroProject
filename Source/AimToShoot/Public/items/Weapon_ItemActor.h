// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemActor.h"
#include "Weapon_ItemActor.generated.h"

/**
 * 
 */
UCLASS()
class AIMTOSHOOT_API AWeapon_ItemActor : public AItemActor
{
	GENERATED_BODY()
public:
	AWeapon_ItemActor();

	void InitType() override;
};
