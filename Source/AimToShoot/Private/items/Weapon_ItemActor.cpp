// Fill out your copyright notice in the Description page of Project Settings.


#include "items/Weapon_ItemActor.h"
void AWeapon_ItemActor::InitType() {
	ItemInfo.ItemID = 2;
	ItemInfo.Quantity = 1;
	ItemInfo.Description = TEXT("This is Weapon");
	ItemInfo.Icon = nullptr;
	ItemInfo.bIsStackable = false;
	ItemInfo.ItemType = EItemType::EIT_Weapon;
}
