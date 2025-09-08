// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DataDefine/AllDataDefine.h"
#include "MyTool.generated.h"

/**
 * 
 */
UCLASS()
class AIMTOSHOOT_API UMyTool : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "BackPack")
	static int32 GetBPItemCount(const TArray<FBackPackStruct>& BackPack, int32 ItemID);
	UFUNCTION(BlueprintCallable, Category = "BackPack")
	static void SortBackPack(TArray<FBackPackStruct>& BackPack,int32 Low,int32 High);
	UFUNCTION(BlueprintCallable, Category = "BackPack")
		static void SortBackPack_A(TArray<FBackPackStruct>& BackPack, int32 Low, int32 High);
	UFUNCTION(BlueprintCallable, Category = "BackPack")
		static void SortBackPack_B(TArray<FBackPackStruct>& BackPack, int32 Low, int32 High);

public:
	//Fast algorithm
	static void FastSort_A(TArray<FBackPackStruct>& Num, int32 low, int32 high);
	static int32 Partition(TArray<FBackPackStruct>& Num, int32 low, int32 high);
	//merge algorithm
	static bool CompareItems_A(const FBackPackStruct& a, const FBackPackStruct& b);
	static bool CompareItems_B(const FBackPackStruct& a, const FBackPackStruct& b);
	template<typename Compare>
	static void merge(TArray<FBackPackStruct>& num, int32 left, int32 mid, int32 right,Compare comp);
	template<typename Compare>
	static void merge_sort(TArray<FBackPackStruct>& num, int32 left, int32 right, Compare comp);
	//radixSort algorihm
	static void radixSortLSD(TArray<FBackPackStruct>& num);
	static int32 GetMaxDigits(const TArray<FBackPackStruct>& num);
	static int32 GetDigit(int32 num, int32 d);
};
