// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Hero_Widget.generated.h"


class AHero;
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
	UPROPERTY(Meta = (BindWidget))
		class UBorder* Border_ammo;
	UPROPERTY(Meta = (BindWidget))
		class UTextBlock* Show_ammo_text;


	void UpDateHealth(AHero* Object);

	void UpDateStamina(AHero* Object);

	void UpDateAmmo(AHero* Player);
};
