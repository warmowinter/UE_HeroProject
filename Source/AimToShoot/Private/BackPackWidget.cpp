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

// ��ʼ������������
void UBackPackWidget::InitializeBackPack()
{
    if (!GridPanel_A || !Grid_EquipWeapon) return;

    // װ����
    UItemSlotWidget* EquipSlot = CreateWidget<UItemSlotWidget>(this, SlotWidgetClass);
    EquipSlot->SetGridType(ESlotType::Equipment);
    Grid_EquipWeapon->AddChildToGrid(EquipSlot, 0, 0);
    Wea_SlotWidgets.Add(EquipSlot);
    // ������Ʒ��
    for (int32 i = 0; i < BPCapacity; i++){
        UItemSlotWidget* ItemSlot = CreateWidget<UItemSlotWidget>(this, SlotWidgetClass);
        ItemSlot->SetGridType(ESlotType::BackPack);
        SlotWidgets.Add(ItemSlot);
        int32 Row = i / 3;
        int32 Col = i % 3;
        GridPanel_A->AddChildToGrid(ItemSlot, Row, Col);
    }
}

// ˢ��UI����
void UBackPackWidget::RefreshBackPack(TArray<FBackPackStruct>& Items, TArray<FBackPackStruct>& Wea_Items )
{
    Wea_SlotWidgets[0]->UpdateSlot(&Wea_Items, 0);
    UE_LOG(LogTemp, Log, TEXT("Update Weapon Grid"));

    for (int32 i = 0; i < SlotWidgets.Num(); i++)
    {
        UE_LOG(LogTemp, Log, TEXT("Update item grid"));
        SlotWidgets[i]->UpdateSlot(&Items, i);
    }
}