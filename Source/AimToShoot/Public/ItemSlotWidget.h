// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DataDefine/AllDataDefine.h"
#include "ItemSlotWidget.generated.h"

/**
 * 
 */
class UBackPackWidget;





UCLASS()
class AIMTOSHOOT_API UItemSlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    // 更新格子（传数组指针，避免拷贝）
    void UpdateSlot(TArray<FBackPackStruct>* InItemData, int32 InIndex);

    // 清空格子
    void ClearSlot();

    void SetGridType(ESlotType GetType);

protected:
    virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
    virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
    virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

private:
    // 指向背包数据的指针
    TArray<FBackPackStruct>* BP_Struct = nullptr;

    // 当前格子对应的索引
    int32 SlotIndex = INDEX_NONE;

    // 当前格子类型（背包 / 装备）
    ESlotType SlotType;

    // 当前物品数据（只作显示用）
    FBackPackStruct ModelDate;

    // UI 绑定的组件
    UPROPERTY(meta = (BindWidget))
        class UImage* ItemIcon = nullptr;

    UPROPERTY(meta = (BindWidget))
        class UTextBlock* ItemCount = nullptr;

};
