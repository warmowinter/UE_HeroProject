// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryMangerInstance.h"

#include "MyTool.h"
#include "ItemActor.h"



/***********************************************************************************************************
多播委托事件聚集地

************************************************************************************************************/


void UInventoryMangerInstance::NoticeHero_EquipUpdate(AHero* Player) {
	Player->RemoveWeapon();
	Player->InitializeEquipments();

}



/********************************************************************************************************
背包逻辑事件

*********************************************************************************************************/


void UInventoryMangerInstance::AddItemToInventory(AHero* Player,const FBackPackStruct& NewItem) {
	int32 EmptySlotIndex = -1;
	TArray<FBackPackStruct>& Player_BPnum = Player->GetBackPackArray();

	if (NewItem.bIsStackable) {
		// 先找可堆叠物品
		for (int32 i = 0; i < Player_BPnum.Num()-5; i++) {
			if (Player_BPnum[i].ItemID == NewItem.ItemID && Player_BPnum[i].Quantity < Player_BPnum[i].MaxStackNumber) {
				Player_BPnum[i].Quantity += NewItem.Quantity;
				return;
			}
			if (Player_BPnum[i].ItemID == 0 && EmptySlotIndex == -1) {
				EmptySlotIndex = i;
			}
		}
	}
	else {
		// 不可堆叠物品直接找空位
		for (int32 i = 0; i < Player_BPnum.Num()-5; i++) {
			if (Player_BPnum[i].ItemID == 0) {
				Player_BPnum[i] = NewItem;
				return;
			}
		}
	}

	// 如果可堆叠但没找到相同物品，就用空位
	if (EmptySlotIndex != -1) {
		Player_BPnum[EmptySlotIndex] = NewItem;
	}

	//如果背包已满
	if (EmptySlotIndex == -1) {
		UE_LOG(LogTemp, Log, TEXT("return the Currently picked up Item"));
	}
}

void UInventoryMangerInstance::TryPickUp(AHero* Player)
{
	TArray<AActor*> OverlappingActors;
	Player->PickUpRange->GetOverlappingActors(OverlappingActors, AItemActor::StaticClass());
	UE_LOG(LogTemp, Log, TEXT("TryPickUp Callable"));
	for (AActor* Actor : OverlappingActors) {
		if (AItemActor* Item = Cast<AItemActor>(Actor)) {
			FVector SpawnLocation(0.0f, 0.0f, -999.0f);
			FRotator SpawnRotation(0.0f, 0.0f, 0.0f);
			if (Item->ItemInfo.WeaponNumber == 1) {
				AWeaponBase* wuhu = GetWorld()->SpawnActor<AWeaponBase>(Player->ConfigWeapon1, SpawnLocation, SpawnRotation);
				Item->ItemInfo.Actor_Ptr = wuhu;
			}
			else if (Item->ItemInfo.WeaponNumber == 2) {
				AWeaponBase* wuhu = GetWorld()->SpawnActor<AWeaponBase>(Player->ConfigWeapon2, SpawnLocation, SpawnRotation);
				Item->ItemInfo.Actor_Ptr = wuhu;
			}
			else if (Item->ItemInfo.WeaponNumber == 3) {
				AWeaponBase* wuhu = GetWorld()->SpawnActor<AWeaponBase>(Player->ConfigWeapon3, SpawnLocation, SpawnRotation);
				Item->ItemInfo.Actor_Ptr = wuhu;
			}
			AddItemToInventory(Player , Item->ItemInfo);
			Item->Destroy();
			break;
		}
	}
	DisPlayShowOfArray(Player);//拾取时同步更新对外展示的数组
	OnBackPackWidgetUpdated.Broadcast(Player);
}

void UInventoryMangerInstance::OrganizeBackPack(AHero* Player) {
	if (Player) {
		UMyTool::SortBackPack(Player->GetBackPackArray(), 0, 15);
		DisPlayShowOfArray(Player);//整理时同步更新对外展示的数组
		OnBackPackWidgetUpdated.Broadcast(Player);
		/*Player->BackPackUI->RefreshBackPack(Player->GetBackPackArray(),Player->GetEquipArray());*/
	}
}

void UInventoryMangerInstance::OrganizeBackPack_A(AHero* Player)
{
	if (Player) {
		UMyTool::SortBackPack_A(Player->GetBackPackArray(), 0, 15);
		DisPlayShowOfArray(Player);//整理时同步更新对外展示的数组
		OnBackPackWidgetUpdated.Broadcast(Player);
	}
}

void UInventoryMangerInstance::OrganizeBackPack_B(AHero* Player)
{
	if (Player) {
		UMyTool::SortBackPack_B(Player->GetBackPackArray(), 0, 15);
		DisPlayShowOfArray(Player);//整理时同步更新对外展示的数组
		OnBackPackWidgetUpdated.Broadcast(Player);
	}
}

void UInventoryMangerInstance::ChangePageToWeapon(AHero* Player)
{
	if (!Player)	return;
	Player->GetPageToWeaponStatus() = true;
	Player->GetPageToConsumableStatus() = false;
	DisPlayShowOfArray(Player);
	OnBackPackWidgetUpdated.Broadcast(Player);
}

void UInventoryMangerInstance::ChangePageToConsumable(AHero* Player)
{
	if (!Player)	return;
	Player->GetPageToWeaponStatus() = false;
	Player->GetPageToConsumableStatus() = true;
	DisPlayShowOfArray(Player);
	OnBackPackWidgetUpdated.Broadcast(Player);
}

void UInventoryMangerInstance::ChangePageToAll(AHero* Player)
{
	if (!Player)	return;
	Player->GetPageToWeaponStatus() = false;
	Player->GetPageToConsumableStatus() = false;
	DisPlayShowOfArray(Player);
	OnBackPackWidgetUpdated.Broadcast(Player);
}

void UInventoryMangerInstance::DisPlayShowOfArray(AHero* Player)
{
	bool W_Status = Player->GetPageToWeaponStatus();
	bool C_Status = Player->GetPageToConsumableStatus();

	TArray<FBackPackStruct>& TargeArray = Player->GetDisPlayShowArray();
	TArray<FBackPackStruct>& SourceArray = Player->GetBackPackArray();

	TArray<int32>& BindIndex = Player->GetSourceToTarget();

	int32 I_Capacity = Player->GetBPCapacity();

	TargeArray.Empty();
	TargeArray.SetNum(I_Capacity);

	BindIndex.Empty();
	BindIndex.SetNum(I_Capacity);

	int32 init = 0;
	if (W_Status && !C_Status) {
		for (int32 i = 0; i < I_Capacity; i++) {
			if (SourceArray[i].ItemType == EItemType::EIT_Weapon) {
				BindIndex[init] = i;
				TargeArray[init++] = SourceArray[i];
			}
			if (i >= 16) {
				BindIndex[i] = i;
				TargeArray[i] = SourceArray[i];
			}
		}
	}
	else if (!W_Status && C_Status) {
		for (int32 i = 0; i < I_Capacity; i++) {
			if (SourceArray[i].ItemType == EItemType::EIT_Consumable) {
				BindIndex[init] = i;
				TargeArray[init++] = SourceArray[i];
			}
			if (i >= 16) {
				BindIndex[i] = i;
				TargeArray[i] = SourceArray[i];
			}
		}
	}
	else if (W_Status && C_Status) {
		UE_LOG(LogTemp, Log, TEXT("An error occurred BPStatus"));
	}
	else {
		for (int32 i = 0; i < I_Capacity; i++) {
			BindIndex[i] = i;
			TargeArray[i] = SourceArray[i];
		}

	}
}

void UInventoryMangerInstance::I_OpenBackPack(AHero* Player) {
	APlayerController* PC = Cast<APlayerController>(Player->GetController());

	if (!(Player->GetBPIsOpenStatus())) {
		if (!(Player->BackPackUI))	return;
		Player->BackPackUI->SetVisibility(ESlateVisibility::Visible);
		OnBackPackWidgetUpdated.Broadcast(Player);
		UE_LOG(LogTemp, Log, TEXT("open BackPack"));
		Player->GetBPIsOpenStatus() = true;

		PC->bShowMouseCursor = true;
		FInputModeGameAndUI InputMode;
		InputMode.SetWidgetToFocus(Player->BackPackUI->TakeWidget());
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		PC->SetInputMode(InputMode);
	}
	else {
		Player->BackPackUI->SetVisibility(ESlateVisibility::Hidden);
		Player->GetBPIsOpenStatus() = false;
		PC->bShowMouseCursor = false;
		FInputModeGameOnly InputMode;
		PC->SetInputMode(InputMode);
	}

}

void UInventoryMangerInstance::swap_PlayerArrayIndex(AHero* Player, int32 source_Index, int32 targe_Index) {

	bool W_Status = Player->GetPageToWeaponStatus();
	bool C_Status = Player->GetPageToConsumableStatus();

	TArray<int32> const BindIndex = Player->GetSourceToTarget();

	FBackPackStruct temp_A = (Player->GetBackPackArray())[BindIndex[source_Index]];
	(Player->GetBackPackArray())[BindIndex[source_Index]] = (Player->GetBackPackArray())[BindIndex[targe_Index]];
	(Player->GetBackPackArray())[BindIndex[targe_Index]] = temp_A;


	//FBackPackStruct temp_A = (Player->GetBackPackArray())[source_Index];
	//(Player->GetBackPackArray())[source_Index] = (Player->GetBackPackArray())[targe_Index];
	//(Player->GetBackPackArray())[targe_Index] = temp_A;
	DisPlayShowOfArray(Player);
	//Player->RemoveWeapon();
	//Player->InitializeEquipments();
	Player->SyncEquipmentsWithBackPack();
	OnBackPackWidgetUpdated.Broadcast(Player);


}