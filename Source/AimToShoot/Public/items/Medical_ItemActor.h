// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemActor.h"
#include "Medical_ItemActor.generated.h"

/**
 * 
 */
UCLASS()
class AIMTOSHOOT_API AMedical_ItemActor : public AItemActor
{
	GENERATED_BODY()
public:
	AMedical_ItemActor();

	void InitType() override;
};
