// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemSlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class AIMTOSHOOT_API UItemSlotWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
		class UImage* ItemIcon;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* ItemCount;

	void UpdateSlot(UTexture2D* Icon, int32 Count);
};
