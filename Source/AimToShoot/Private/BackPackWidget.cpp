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

void UBackPackWidget::RefreshBackPack(const TArray<FBackPackStruct>& Items)
{
	if (!GridPanel_A)	return;
	GridPanel_A->ClearChildren();
	for (int32 i = 0; i < BPCapacity; i++) {
		UItemSlotWidget* ItemSlot = CreateWidget<UItemSlotWidget>(this, SlotWidgetClass);
		if (i < Items.Num() && Items[i].ItemType != EItemType::EIT_Unknown) {
			ItemSlot->UpdateSlot(Items[i].Icon, Items[i].Quantity);
			int32 Row = i / 3;
			int32 Col = i % 3;
			GridPanel_A->AddChildToGrid(ItemSlot, Row, Col);
		}
		else {
			ItemSlot->UpdateSlot(nullptr, 0);
			int32 Row = i / 3;
			int32 Col = i % 3;
			GridPanel_A->AddChildToGrid(ItemSlot, Row, Col);
		}
	}
}

void UBackPackWidget::InitializeBackPack() {
	if (!GridPanel_A)	return;
	UE_LOG(LogTemp,Log,TEXT("InitializeBackPack"))
	for (int32 i = 0; i < BPCapacity; i++) {
		UE_LOG(LogTemp, Log, TEXT("InitializeGrid"))
		UItemSlotWidget* ItemSlot = CreateWidget<UItemSlotWidget>(this, SlotWidgetClass);
		ItemSlot->UpdateSlot(nullptr, 0);
		int32 Row = i / 3;
		int32 Col = i % 3;
		GridPanel_A->AddChildToGrid(ItemSlot, Row, Col);
	}
}