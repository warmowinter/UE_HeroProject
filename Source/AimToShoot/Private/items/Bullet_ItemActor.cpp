// Fill out your copyright notice in the Description page of Project Settings.


#include "items/Bullet_ItemActor.h"

void ABullet_ItemActor::InitType() {
	ItemInfo.ItemID = 1;
	ItemInfo.Quantity = 1;
	ItemInfo.Description = TEXT("This is Bullet");
	ItemInfo.Icon = nullptr;
	ItemInfo.bIsStackable = true;
	ItemInfo.ItemType = EItemType::EIT_Consumable;
}
