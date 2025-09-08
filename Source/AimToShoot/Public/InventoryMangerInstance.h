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
	UFUNCTION(BlueprintCallable, Category = "OrganizeBackPack")
		void OrganizeBackPack_A(AHero* Player);
	UFUNCTION(BlueprintCallable, Category = "OrganizeBackPack")
		void OrganizeBackPack_B(AHero* Player);

	UFUNCTION(BlueprintCallable, Category = "ChangeBPStatus")
		void ChangePageToWeapon(AHero* Player);
	UFUNCTION(BlueprintCallable, Category = "ChangeBPStatus")
		void ChangePageToConsumable(AHero* Player);
	UFUNCTION(BlueprintCallable, Category = "ChangeBPStatus")
		void ChangePageToAll(AHero* Player);

	//处理选择性显示背包数组
	UFUNCTION(BlueprintCallable, Category = "DisplayArrayShow")
		void DisPlayShowOfArray(AHero* Player);
public:

	//背包选择展示状态,其实我在这想了个问题，这个背包管理系统是单例模式，那么应该有初始化相关函数，我仅是在这头文件进行初始化。
	//那么在以后存储游戏状态时，应该还要做一些改动，
	//思考了下，不能在背包管理系统做存储变量状态，这仅是对收到背包输入做逻辑处理，如若在这进行存储状态，那么在制作多人游戏时该背包状态即是多人共享了
	// 
	// 以下是输入的，先注释掉，避免日后误会
	// 
	// 	//背包状态
	//UPROPERTY()
	//	bool I_Is_OpenBP = false;
	//UPROPERTY()
	//	bool I_Is_ChoiceWeapons = false;	
	//UPROPERTY()
	//	bool I_Is_ChoiceConsumables = false;



};
