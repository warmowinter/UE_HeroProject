// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AllDataDefine.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EItemType : uint8 {
	EIT_Unknown UMETA(DIsplayName = "Unknown"),
	EIT_Consumable UMETA(DisplayName = "Consumable"),
	EIT_Weapon UMETA(DisplayName = "Weapon"),
	EIT_Armor UMETA(DisplayName = "Armor"),
	EIT_Material UMETA(DisplayName = "Material"),
	EIT_QuestItem UMETA(DisplayName = "Quest Item")
};



USTRUCT(BlueprintType)
struct FBackPackStruct {
	GENERATED_BODY()
public:
	FBackPackStruct() :ItemID(0)
		, Quantity(1)
		, Description(TEXT("Unknown Item"))
		, Icon(nullptr)
		, bIsStackable(false)
		, ItemType(EItemType::EIT_Unknown)
	{}
	//��ƷID
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		int32 ItemID;
	//����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		int32 Quantity;
	//����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		FString Description;
	//ͼ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		UTexture2D* Icon;
	//�ѵ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		bool bIsStackable;
	//����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		EItemType ItemType;
};


UCLASS()
class AIMTOSHOOT_API UAllDataDefine : public UObject
{
	GENERATED_BODY()
	
};
