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
    // ���¸��ӣ�������ָ�룬���⿽����
    void UpdateSlot(TArray<FBackPackStruct>* InItemData, int32 InIndex);

    // ��ո���
    void ClearSlot();

    void SetGridType(ESlotType GetType);

protected:
    virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
    virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
    virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

private:
    // ָ�򱳰����ݵ�ָ��
    TArray<FBackPackStruct>* BP_Struct = nullptr;

    // ��ǰ���Ӷ�Ӧ������
    int32 SlotIndex = INDEX_NONE;

    // ��ǰ�������ͣ����� / װ����
    ESlotType SlotType;

    // ��ǰ��Ʒ���ݣ�ֻ����ʾ�ã�
    FBackPackStruct ModelDate;

    // UI �󶨵����
    UPROPERTY(meta = (BindWidget))
        class UImage* ItemIcon = nullptr;

    UPROPERTY(meta = (BindWidget))
        class UTextBlock* ItemCount = nullptr;

};
