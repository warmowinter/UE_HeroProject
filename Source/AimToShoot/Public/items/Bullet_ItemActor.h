// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemActor.h"
#include "Bullet_ItemActor.generated.h"

/**
 * 
 */
UCLASS()
class AIMTOSHOOT_API ABullet_ItemActor : public AItemActor
{
	GENERATED_BODY()
public:
	ABullet_ItemActor();

	void InitType() override;
};
