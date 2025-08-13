// Fill out your copyright notice in the Description page of Project Settings.


#include "items/Bullet_ItemActor.h"

ABullet_ItemActor::ABullet_ItemActor() {
	ConstructorHelpers::FObjectFinder<UTexture2D> Icon_Bul(TEXT("Texture2D'/Game/Current_make/Assets/bullets_s.bullets_s'"));
	if (Icon_Bul.Succeeded()) {
		ItemTexture2D = Icon_Bul.Object;
	}

}

void ABullet_ItemActor::InitType() {
	ItemInfo.ItemID = 1;
	ItemInfo.Quantity = 1;
	ItemInfo.Description = TEXT("This is Bullet");
	ItemInfo.Icon = ItemTexture2D;
	ItemInfo.bIsStackable = true;
	ItemInfo.ItemType = EItemType::EIT_Armor;
	ItemInfo.WeaponNumber = 0;
}
