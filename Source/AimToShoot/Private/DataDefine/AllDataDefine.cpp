// Fill out your copyright notice in the Description page of Project Settings.


#include "DataDefine/AllDataDefine.h"


const TMap<EItemType, int32> UAllDataDefine::ItemTypeSortWeights = {
	{ EItemType::EIT_Weapon, 1 },       // 武器
	{ EItemType::EIT_Armor, 2 },        // 防具次之
	{ EItemType::EIT_Consumable, 3 },   // 消耗品
	{ EItemType::EIT_QuestItem, 4 },    // 任务物品
	{ EItemType::EIT_Material, 5 },     // 材料
	{ EItemType::EIT_Unknown, 999 }       // 未知物品
};


int32 UAllDataDefine::GetItemTypeSortWeight(EItemType ItemType)
{
	
	const int32* Weight = ItemTypeSortWeights.Find(ItemType);
	return Weight ? *Weight : 0;
}