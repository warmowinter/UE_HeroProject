// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Delegates/DelegateCombinations.h"
#include "DataDefine/AllDataDefine.h"
#include "Hero.h"
#include "InventoryMangerInstance.generated.h"


class AHero;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBackPackWidgetUpdated, AHero*, Player);

/**
 * 
 */
UCLASS()
class AIMTOSHOOT_API UInventoryMangerInstance : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
		FOnBackPackWidgetUpdated OnBackPackWidgetUpdated;

	void NoticeHero_EquipUpdate(AHero* Player);


	UFUNCTION(BlueprintCallable, Category = "AddItemToInventory")
		void AddItemToInventory(AHero* Player, const FBackPackStruct& NewItem);
	UFUNCTION(BlueprintCallable, Category = "TryPickUp")
		void TryPickUp(AHero* Player);
	UFUNCTION()
		void I_OpenBackPack(AHero* Player);
	UFUNCTION()
		void swap_PlayerArrayIndex(AHero* Player, int32 source_Index, int32 targe_Index);

	UFUNCTION(BlueprintCallable, Category = "OrganizeBackPack")
		void OrganizeBackPack(AHero* Player);



public:
	//±³°ü×´Ì¬
	UPROPERTY()
		bool I_Is_OpenBP = false;

};
