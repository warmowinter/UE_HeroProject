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
	OnBackPackWidgetUpdated.Broadcast(Player);
}

void UInventoryMangerInstance::OrganizeBackPack(AHero* Player) {
	if (Player) {
		UMyTool::SortBackPack(Player->GetBackPackArray(), 0, 15);
		OnBackPackWidgetUpdated.Broadcast(Player);
		/*Player->BackPackUI->RefreshBackPack(Player->GetBackPackArray(),Player->GetEquipArray());*/
	}
}

void UInventoryMangerInstance::I_OpenBackPack(AHero* Player) {
	APlayerController* PC = Cast<APlayerController>(Player->GetController());

	if (!I_Is_OpenBP) {
		if (!(Player->BackPackUI))	return;
		Player->BackPackUI->SetVisibility(ESlateVisibility::Visible);
		OnBackPackWidgetUpdated.Broadcast(Player);
		UE_LOG(LogTemp, Log, TEXT("open BackPack"));
		I_Is_OpenBP = true;

		PC->bShowMouseCursor = true;
		FInputModeGameAndUI InputMode;
		InputMode.SetWidgetToFocus(Player->BackPackUI->TakeWidget());
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		PC->SetInputMode(InputMode);
	}
	else {
		Player->BackPackUI->SetVisibility(ESlateVisibility::Hidden);
		I_Is_OpenBP = false;
		PC->bShowMouseCursor = false;
		FInputModeGameOnly InputMode;
		PC->SetInputMode(InputMode);
	}

}

void UInventoryMangerInstance::swap_PlayerArrayIndex(AHero* Player, int32 source_Index, int32 targe_Index) {
	FBackPackStruct temp_A = (Player->GetBackPackArray())[source_Index];
	(Player->GetBackPackArray())[source_Index] = (Player->GetBackPackArray())[targe_Index];
	(Player->GetBackPackArray())[targe_Index] = temp_A;
	//Player->RemoveWeapon();
	//Player->InitializeEquipments();
	Player->SyncEquipmentsWithBackPack();
	OnBackPackWidgetUpdated.Broadcast(Player);


}