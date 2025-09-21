// Fill out your copyright notice in the Description page of Project Settings.


#include "Hero_Widget.h"
#include "Hero.h"
#include "Components/ProgressBar.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"

void UHero_Widget::UpDateHealth(AHero* Object) {
	if (HealthBar) {
		HealthBar->SetPercent(1.0f);
	}
	
}

void UHero_Widget::UpDateStamina(AHero* Object) {

	if (!Object || !StaminaBar)	return;

	float Cur_Stam = Object->CurrentStamina;
	float Max_Stam = Object->MaxStamina;
	float cal_Per = (Max_Stam > 0.0f) ? (Cur_Stam / Max_Stam) : 0.0f;

	StaminaBar->SetPercent(cal_Per);
}

void UHero_Widget::UpDateAmmo(AHero* Player)
{
	if (!Player) return;
	if (Player->CurrentWeapon == nullptr) {
		Border_ammo->SetVisibility(ESlateVisibility::Hidden);
	}
	else {
		Border_ammo->SetVisibility(ESlateVisibility::Visible);
		Show_ammo_text->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), Player->CurrentWeapon->GetWeaponCurrentAmmo(), Player->CurrentWeapon->GetWeaponMaxAmmo())));
	}
}




