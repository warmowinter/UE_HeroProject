// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "InventoryMangerInstance.generated.h"

/**
 * 
 */
UCLASS()
class AIMTOSHOOT_API UInventoryMangerInstance : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	void NoticeHero_EquipUpdate();

public:
	UPROPERTY()
		class AHero* Bind_Chara;
};
