// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryMangerInstance.h"
#include "Hero.h"

void UInventoryMangerInstance::NoticeHero_EquipUpdate() {
	Bind_Chara->RemoveWeapon();
	Bind_Chara->EquipWeapon();

}