// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemSlotWidget.h"
#include "BackPackWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/DragDropOperation.h"
#include "InventoryMangerInstance.h"

void UItemSlotWidget::UpdateSlot(TArray<FBackPackStruct>* InItemData, int32 InIndex)
{
    BP_Struct = InItemData;
    SlotIndex = InIndex;

    if (BP_Struct && BP_Struct->IsValidIndex(SlotIndex)){
        ModelDate = (*BP_Struct)[SlotIndex];
    }
    else{
        ModelDate = FBackPackStruct(); 
    }
    if (ModelDate.Icon) {
        UE_LOG(LogTemp, Log, TEXT("Show Icon"));
        ItemIcon->SetBrushFromTexture(ModelDate.Icon);
    }
    else {
        ItemIcon->SetBrushFromTexture(nullptr);
        UE_LOG(LogTemp, Log, TEXT(" Icon  is nullptr"));
    }     
    if (ModelDate.Quantity) {
        ItemCount->SetText(ModelDate.Quantity > 1 ? FText::AsNumber(ModelDate.Quantity) : FText::GetEmpty());
    }
}
//����
void UItemSlotWidget::ClearSlot(){
    if (BP_Struct && BP_Struct->IsValidIndex(SlotIndex)){
        (*BP_Struct)[SlotIndex] = FBackPackStruct(); // ��������
        UpdateSlot(BP_Struct, SlotIndex);  // ˢ����ʾ
    }
}

void UItemSlotWidget::SetGridType(ESlotType GetType) {
    SlotType = GetType;
}

//����������
FReply UItemSlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent){
    if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton){
        UE_LOG(LogTemp, Log, TEXT("press of mouse left"));
        return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
    }
    return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}
//�����ק
void UItemSlotWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation){
    if (!ModelDate.Icon) return;

    UDragDropOperation* DragOperation = UWidgetBlueprintLibrary::CreateDragDropOperation(UDragDropOperation::StaticClass());

    UItemSlotWidget* DragVisual = CreateWidget<UItemSlotWidget>(GetWorld(), GetClass());
    DragVisual->UpdateSlot(BP_Struct, SlotIndex);

    DragOperation->DefaultDragVisual = DragVisual;
    DragOperation->Payload = this;
    OutOperation = DragOperation;
}
//�������ɿ�
bool UItemSlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation){
    if (UItemSlotWidget* FromSlot = Cast<UItemSlotWidget>(InOperation->Payload)){
        UE_LOG(LogTemp, Log, TEXT("song kai shu biao zuo jian"));

        if (BP_Struct && FromSlot->BP_Struct &&
            BP_Struct->IsValidIndex(SlotIndex) &&
            FromSlot->BP_Struct->IsValidIndex(FromSlot->SlotIndex)){
            UE_LOG(LogTemp, Log, TEXT("man zu jiao huan shu ju"));
            // ��������
            FBackPackStruct Temp = (*BP_Struct)[SlotIndex];
            (*BP_Struct)[SlotIndex] = (*FromSlot->BP_Struct)[FromSlot->SlotIndex];
            (*FromSlot->BP_Struct)[FromSlot->SlotIndex] = Temp;

            // ˢ����������
            UpdateSlot(BP_Struct, SlotIndex);
            FromSlot->UpdateSlot(FromSlot->BP_Struct, FromSlot->SlotIndex);

            if (GetWorld() && GetWorld()->GetGameInstance()){
                UInventoryMangerInstance* BPSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UInventoryMangerInstance>();
                if (BPSubsystem){
                    BPSubsystem->NoticeHero_EquipUpdate();
                }
            }

            return true;
        }
    }
    return false;
}