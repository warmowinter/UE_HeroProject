// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemSlotWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h" 

void UItemSlotWidget::UpdateSlot(UTexture2D* Icon, int32 Count) {
	if (ItemIcon && Icon) {
		ItemIcon->SetBrushFromTexture(Icon);
	}
	if (ItemCount) {
		ItemCount->SetText(FText::AsNumber(Count));
	}
}