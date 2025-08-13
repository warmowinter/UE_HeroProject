// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BackPackSubSystem.generated.h"

/**
 * 
 */
UCLASS()
class AIMTOSHOOT_API UBackPackSubSystem : public UObject
{
	GENERATED_BODY()
public:
	void NoticeHero_EquipUpdate();

public:
	UPROPERTY()
		class AHero* Bind_Chara;
};
