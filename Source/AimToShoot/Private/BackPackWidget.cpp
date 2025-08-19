// Fill out your copyright notice in the Description page of Project Settings.


#include "BackPackWidget.h"
#include "Components/GridPanel.h"

UBackPackWidget::UBackPackWidget(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer) {
	BPCapacity = 12;

}
void UBackPackWidget::NativeConstruct() {
	Super::NativeConstruct();
	InitializeBackPack();
}

// 初始化：创建格子
void UBackPackWidget::InitializeBackPack()
{
    if (!GridPanel_A || !Grid_EquipWeapon) return;

    // 装备格
    for (int32 i = 0; i < 6; i++) {
        UItemSlotWidget* EquipSlot = CreateWidget<UItemSlotWidget>(this, SlotWidgetClass);
        EquipSlot->SetGridType(ESlotType::Equipment);
        int32 Row = i / 2;
        int32 Col = i % 2;
        Grid_EquipWeapon->AddChildToGrid(EquipSlot, Row,Col);
        Wea_SlotWidgets.Add(EquipSlot);
    }
    // 背包物品格
    for (int32 i = 0; i < BPCapacity; i++){
        UItemSlotWidget* ItemSlot = CreateWidget<UItemSlotWidget>(this, SlotWidgetClass);
        ItemSlot->SetGridType(ESlotType::BackPack);
        SlotWidgets.Add(ItemSlot);
        int32 Row = i / 3;
        int32 Col = i % 3;
        GridPanel_A->AddChildToGrid(ItemSlot, Row, Col);
    }
}

// 刷新UI数据
void UBackPackWidget::RefreshBackPack(TArray<FBackPackStruct>& Items, TArray<FBackPackStruct>& Wea_Items )
{
   
    for (int32 j = 0; j < Wea_SlotWidgets.Num(); j++) {
        Wea_SlotWidgets[j]->UpdateSlot(&Wea_Items, j);
        UE_LOG(LogTemp, Log, TEXT("Update Weapon Grid"));
    }


    for (int32 i = 0; i < SlotWidgets.Num(); i++)
    {
        UE_LOG(LogTemp, Log, TEXT("Update item grid"));
        SlotWidgets[i]->UpdateSlot(&Items, i);
    }
}