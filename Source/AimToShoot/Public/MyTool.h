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
	static void SortBackPack(TArray<FBackPackStruct>& BackPack);

};
