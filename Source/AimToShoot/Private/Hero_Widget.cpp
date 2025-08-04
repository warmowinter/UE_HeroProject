// Fill out your copyright notice in the Description page of Project Settings.


#include "Hero_Widget.h"
#include "Components/ProgressBar.h"

void UHero_Widget::UpDateHealth(float Percent) {
	if (HealthBar) {
		HealthBar->SetPercent(Percent);
	}
}

void UHero_Widget::UpDateStamina(float Percent) {
	if (StaminaBar) {
		StaminaBar->SetPercent(Percent);
	}
}