// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Hero_Widget.generated.h"

/**
 * 
 */
UCLASS()
class AIMTOSHOOT_API UHero_Widget : public UUserWidget
{
	GENERATED_BODY()
	

public:
	UPROPERTY(Meta = (BindWidget))
		class UProgressBar* HealthBar;
	UPROPERTY(Meta = (BindWidget))
		class UProgressBar* StaminaBar;

	void UpDateHealth(float Percent);

	void UpDateStamina(float percent);
};
