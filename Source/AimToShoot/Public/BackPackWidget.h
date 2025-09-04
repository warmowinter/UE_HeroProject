// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DataDefine/AllDataDefine.h"
#include "Delegates/DelegateCombinations.h"
#include "BackPackWidget.generated.h"

/**
 * 
 */


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOrganizeButton, UBackPackWidget*, BP_UI);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnSwapIndex_Array, UBackPackWidget*, BP_UI, int32, source_Index, int32, targe_Index);


class UInventoryMangerInstance;
class UItemSlotWidget;

UCLASS()
class AIMTOSHOOT_API UBackPackWidget : public UUserWidget
{
	GENERATED_BODY()

		
public:
	UBackPackWidget(const FObjectInitializer& ObjectInitializer);
	void NativeConstruct()override;

	UPROPERTY(BlueprintAssignable)
		FOnOrganizeButton OnOrganizePress;
	UPROPERTY(BlueprintAssignable)
		FOnSwapIndex_Array OnSwapIndex_Array;

public:
	UPROPERTY(Meta = (BindWidget))
		class UGridPanel* GridPanel_A;

	UPROPERTY(Meta = (BindWidget))
		class UButton* Button_Organize;
	UPROPERTY(Meta = (BindWidget))
		class UButton* Button_Quality;
	UPROPERTY(Meta = (BindWidget))
		class UButton* Button_Num;
	UPROPERTY(Meta = (BindWidget))
		class UButton* Button_Weapon;
	UPROPERTY(Meta = (BindWidget))
		class UButton* Button_Consumable;

	UPROPERTY(Meta = (BindWidget))
		class UBorder* first;
	UPROPERTY(Meta = (BindWidget))
		class UBorder* second;
	UPROPERTY(Meta = (BindWidget))
		class UBorder* three;
	UPROPERTY(Meta = (BindWidget))
		class UBorder* four;
	UPROPERTY(Meta = (BindWidget))
		class UBorder* five;

	UPROPERTY(Meta = (BindWidget))
		UItemSlotWidget* Slot_Weapon1;//ÎäÆ÷²å²Û1
	UPROPERTY(Meta = (BindWidget))
		UItemSlotWidget* Slot_Weapon2;//ÎäÆ÷²å²Û2
	UPROPERTY(Meta = (BindWidget))
		UItemSlotWidget* Slot_Weapon3;//ÎäÆ÷²å²Û3
	UPROPERTY(Meta = (BindWidget))
		UItemSlotWidget* Slot_Armor;//»¤¼×²å²Û
	UPROPERTY(Meta = (BindWidget))
		UItemSlotWidget* Slot_Helmet;//Í·¿ø²å²Û

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
		TSubclassOf<UItemSlotWidget> SlotWidgetClass;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "BackPackNum")
		TArray<FBackPackStruct> BackPackNum;

	TArray<UItemSlotWidget*> SlotWidgets;
	TArray<UItemSlotWidget*> Wea_SlotWidgets;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BackPackCapacity")
		int32 BPCapacity;

	//±³°ü¹ÜÀíÏµÍ³
	UPROPERTY()
		UInventoryMangerInstance* BPToSubsystem;
public:
	UFUNCTION()
		void OnOrganizeButtonClicked();
	UFUNCTION()
		void OnQualityButtonClicked();
	UFUNCTION()
		void OnNumButtonClicked();
	UFUNCTION()
		void OnWeaponButtonClicked();
	UFUNCTION()
		void OnConsumableButtonClicked();

	UFUNCTION()
		void HandleGridSwapIndex(UBackPackWidget* BP_UI, int32 source_Index, int32 targe_Index);
		
	UFUNCTION(BlueprintCallable, Category = "RefreshBackPack")
		void RefreshBackPack(AHero* Player);



	UFUNCTION()
		void InitializeBackPack();


};
