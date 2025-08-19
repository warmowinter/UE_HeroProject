// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AllDataDefine.generated.h"



class AWeaponBase;

/**
 * 
 */
//��������
UENUM(BlueprintType)
enum class ESlotType :uint8 {
	BackPack,
	Equipment
};


//��Ʒ����
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
		, MaxStackNumber(30)
		, ItemType(EItemType::EIT_Unknown)
		, WeaponNumber(0)
		, Actor_Ptr(nullptr)

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
	//���ѵ�����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		int32 MaxStackNumber;
	//����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		EItemType ItemType;
	//�������֣���������
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		int32 WeaponNumber;
	//����ָ��-ָ��ʵ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		AWeaponBase* Actor_Ptr;
};


UCLASS()
class AIMTOSHOOT_API UAllDataDefine : public UObject
{
	GENERATED_BODY()
	
};
