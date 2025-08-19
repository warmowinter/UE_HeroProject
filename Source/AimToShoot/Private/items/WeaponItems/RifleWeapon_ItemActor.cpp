// Fill out your copyright notice in the Description page of Project Settings.


#include "items/WeaponItems/RifleWeapon_ItemActor.h"

ARifleWeapon_ItemActor::ARifleWeapon_ItemActor() {
	ConstructorHelpers::FObjectFinder<UTexture2D> Icon_Wea(TEXT("Texture2D'/Game/Current_make/Assets/Weapon_s.Weapon_s'"));
	if (Icon_Wea.Succeeded()) {
		ItemTexture2D = Icon_Wea.Object;
	}
}


void ARifleWeapon_ItemActor::InitType() {
	ItemInfo.ItemID = 2;
	ItemInfo.Quantity = 1;
	ItemInfo.Description = TEXT("This is RifleWeapon");
	ItemInfo.Icon = ItemTexture2D;
	ItemInfo.bIsStackable = false;
	ItemInfo.ItemType = EItemType::EIT_Weapon;
	ItemInfo.WeaponNumber = 1;
	ItemInfo.MaxStackNumber = 1;
}