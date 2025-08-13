// Fill out your copyright notice in the Description page of Project Settings.


#include "items/Medical_ItemActor.h"

AMedical_ItemActor::AMedical_ItemActor() {
	ConstructorHelpers::FObjectFinder<UTexture2D> IconObj(TEXT("Texture2D'/Game/Current_make/Assets/OIP.OIP'"));
	if (IconObj.Succeeded())
	{
		ItemTexture2D = IconObj.Object;
	}

}

void AMedical_ItemActor::InitType() {
	ItemInfo.ItemID = 3;
	ItemInfo.Quantity = 1;
	ItemInfo.Description = TEXT("This is Medical");
	ItemInfo.Icon = ItemTexture2D;
	ItemInfo.bIsStackable = true;
	ItemInfo.ItemType = EItemType::EIT_Consumable;
	ItemInfo.WeaponNumber = 0;
}