// Fill out your copyright notice in the Description page of Project Settings.


#include "items/WeaponItems/PistolWeapon_ItemActor.h"

APistolWeapon_ItemActor::APistolWeapon_ItemActor() {
	ConstructorHelpers::FObjectFinder<UTexture2D> Icon_Wea(TEXT("Texture2D'/Game/Current_make/Assets/PistolWeaponIcon.PistolWeaponIcon'"));
	if (Icon_Wea.Succeeded()) {
		ItemTexture2D = Icon_Wea.Object;
	}
}

void APistolWeapon_ItemActor::InitType()
{
	ItemInfo.ItemID = 5;
	ItemInfo.Quantity = 1;
	ItemInfo.Description = TEXT("This is PistolWeapon");
	ItemInfo.Icon = ItemTexture2D;
	ItemInfo.bIsStackable = false;
	ItemInfo.ItemType = EItemType::EIT_Weapon;
	ItemInfo.WeaponNumber = 2;
	ItemInfo.MaxStackNumber = 1;
}
