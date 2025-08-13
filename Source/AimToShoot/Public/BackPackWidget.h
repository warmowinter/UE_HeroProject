// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemSlotWidget.h"
#include "DataDefine/AllDataDefine.h"
#include "BackPackWidget.generated.h"

/**
 * 
 */
UCLASS()
class AIMTOSHOOT_API UBackPackWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UBackPackWidget(const FObjectInitializer& ObjectInitializer);
	void NativeConstruct()override;
public:
	UPROPERTY(Meta = (BindWidget))
		class UGridPanel* GridPanel_A;
	UPROPERTY(Meta = (BindWidget))
		class UGridPanel* Grid_EquipWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
		TSubclassOf<UItemSlotWidget> SlotWidgetClass;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "BackPackNum")
		TArray<FBackPackStruct> BackPackNum;

	TArray<UItemSlotWidget*> SlotWidgets;
	TArray<UItemSlotWidget*> Wea_SlotWidgets;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BackPackCapacity")
		int32 BPCapacity;


public:
	UFUNCTION(BlueprintCallable, Category = "RefreshBackPack")
		void RefreshBackPack(TArray<FBackPackStruct>& Items,TArray<FBackPackStruct>& Wea_Items);

	UFUNCTION()
		void InitializeBackPack();
};
