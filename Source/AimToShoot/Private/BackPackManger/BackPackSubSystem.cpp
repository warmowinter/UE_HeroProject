// Fill out your copyright notice in the Description page of Project Settings.


#include "BackPackManger/BackPackSubSystem.h"
#include "Hero.h"
#include "ItemSlotWidget.h"

void UBackPackSubSystem::NoticeHero_EquipUpdate() {
	Bind_Chara->EquipWeapon();
}