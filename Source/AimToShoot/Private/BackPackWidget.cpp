// Fill out your copyright notice in the Description page of Project Settings.


#include "BackPackWidget.h"
#include "Components/GridPanel.h"
#include "Components/Button.h"
#include "Components/Border.h"
#include "ItemSlotWidget.h"
#include "InventoryMangerInstance.h"

UBackPackWidget::UBackPackWidget(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer) {
	BPCapacity = 21;

}
void UBackPackWidget::NativeConstruct() {
	Super::NativeConstruct();
	InitializeBackPack();

    if (Button_Organize) {
        Button_Organize->OnClicked.AddDynamic(this, &UBackPackWidget::OnOrganizeButtonClicked);
    }

    if (Button_Quality) {
        Button_Quality->OnClicked.AddDynamic(this, &UBackPackWidget::OnQualityButtonClicked);
    }

    if (Button_Num) {
        Button_Num->OnClicked.AddDynamic(this, &UBackPackWidget::OnNumButtonClicked);
    }

    if (Button_Weapon) {
        Button_Weapon->OnClicked.AddDynamic(this, &UBackPackWidget::OnWeaponButtonClicked);
    }

    if (Button_Consumable) {
        Button_Consumable->OnClicked.AddDynamic(this, &UBackPackWidget::OnConsumableButtonClicked);
    }

    if (Button_All) {
        Button_All->OnClicked.AddDynamic(this, &UBackPackWidget::OnChoiceAllButtonClicked);
    }

    BPToSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UInventoryMangerInstance>();
    BPToSubsystem->OnBackPackWidgetUpdated.AddDynamic(this, &UBackPackWidget::RefreshBackPack);

}

// 初始化：创建格子
void UBackPackWidget::InitializeBackPack()
{
    if (!GridPanel_A) return;

    //// 装备格
    //for (int32 i = 0; i < 6; i++) {
    //    UItemSlotWidget* EquipSlot = CreateWidget<UItemSlotWidget>(this, SlotWidgetClass);
    //    EquipSlot->SetGridType(ESlotType::Equipment);
    //    int32 Row = i / 8;
    //    int32 Col = i % 8;
    //    Grid_EquipWeapon->AddChildToGrid(EquipSlot, Row,Col);
    //    Wea_SlotWidgets.Add(EquipSlot);
    //}

    // 背包物品格
    for (int32 i = 0; i < BPCapacity-5; i++){
        UItemSlotWidget* ItemSlot = CreateWidget<UItemSlotWidget>(this, SlotWidgetClass);
        ItemSlot->SetGridType(ESlotType::BackPack);
        ItemSlot->OwnerBackPack = this;
        ItemSlot->OnDropGridInput.AddDynamic(this, &UBackPackWidget::HandleGridSwapIndex);
        SlotWidgets.Add(ItemSlot);
        int32 Row = i / 8;
        int32 Col = i % 8;
        GridPanel_A->AddChildToGrid(ItemSlot, Row, Col);
    }


 /**********************************************************************************************************************
 拖拽完装备特殊格子广播-交由背包系统管理
 
************************************************************************************************************************/
    Slot_Weapon1->SetGridType(ESlotType::Equipment);
    Slot_Weapon1->OwnerBackPack = this;
    Slot_Weapon1->OnDropGridInput.AddDynamic(this, &UBackPackWidget::HandleGridSwapIndex);

    Slot_Weapon2->SetGridType(ESlotType::Equipment);
    Slot_Weapon2->OwnerBackPack = this;
    Slot_Weapon2->OnDropGridInput.AddDynamic(this, &UBackPackWidget::HandleGridSwapIndex);

    Slot_Weapon3->SetGridType(ESlotType::Equipment);
    Slot_Weapon3->OwnerBackPack = this;
    Slot_Weapon3->OnDropGridInput.AddDynamic(this, &UBackPackWidget::HandleGridSwapIndex);

    Slot_Armor->SetGridType(ESlotType::Equipment);
    Slot_Armor->OwnerBackPack = this;
    Slot_Armor->OnDropGridInput.AddDynamic(this, &UBackPackWidget::HandleGridSwapIndex);

    Slot_Helmet->SetGridType(ESlotType::Equipment);
    Slot_Helmet->OwnerBackPack = this;
    Slot_Helmet->OnDropGridInput.AddDynamic(this, &UBackPackWidget::HandleGridSwapIndex);

    SlotWidgets.Add(Slot_Weapon1);
    SlotWidgets.Add(Slot_Weapon2);
    SlotWidgets.Add(Slot_Weapon3);
    SlotWidgets.Add(Slot_Armor);
    SlotWidgets.Add(Slot_Helmet);

}

/**********************************************************************************************************************
背包界面按钮点击触发广播事件
*
************************************************************************************************************************/

void UBackPackWidget::OnOrganizeButtonClicked()
{
    OnOrganizePress.Broadcast(this);

}

void UBackPackWidget::OnQualityButtonClicked()
{
    OnOrganize_APress.Broadcast(this);
    UE_LOG(LogTemp, Log, TEXT("Quality sort"));
}

void UBackPackWidget::OnNumButtonClicked()
{
    OnOrganize_BPress.Broadcast(this);
    UE_LOG(LogTemp, Log, TEXT("NUm sort"));
}

void UBackPackWidget::OnWeaponButtonClicked()
{
    OnChoiceWeaponsButton.Broadcast();
    UE_LOG(LogTemp, Log, TEXT("Choice Weapon Categoty"));
}

void UBackPackWidget::OnConsumableButtonClicked()
{
    OnChoiceConsumablesButton.Broadcast();
    UE_LOG(LogTemp, Log, TEXT("Choice Consumable Categoty"));
}

void UBackPackWidget::OnChoiceAllButtonClicked()
{
    OnChoiceAllButton.Broadcast();
    UE_LOG(LogTemp, Log, TEXT("Choice All Categoty"));
}

void UBackPackWidget::HandleGridSwapIndex(UBackPackWidget* BP_UI, int32 source_Index, int32 targe_Index)
{
    if (BP_UI == this) {
        OnSwapIndex_Array.Broadcast(this, source_Index, targe_Index);
    }
}

// 刷新UI数据
void UBackPackWidget::RefreshBackPack(AHero* Player)
{
    //for (int32 j = 0; j < Wea_SlotWidgets.Num(); j++) {
    //    Wea_SlotWidgets[j]->UpdateSlot(&(Player->GetEquipArray()), j);
    //    UE_LOG(LogTemp, Log, TEXT("Update Weapon Grid"));
    //}

    for (int32 i = 0; i < SlotWidgets.Num(); i++)
    {
        UE_LOG(LogTemp, Log, TEXT("Update item grid"));
        SlotWidgets[i]->UpdateSlot(&(Player->GetDisPlayShowArray()), i);
    }
}