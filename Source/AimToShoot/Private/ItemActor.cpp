// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemActor.h"

// Sets default values
AItemActor::AItemActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
	SkeleMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeleMesh->SetupAttachment(RootComponent);
	//ÎÆÀí³õÊ¼»¯s
	ItemTexture2D = nullptr;

}

// Called when the game starts or when spawned
void AItemActor::BeginPlay()
{
	Super::BeginPlay();
	InitType();
	InitVisible(ItemInfo);
}

// Called every frame
void AItemActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItemActor::InitType() {
	ItemInfo.ItemID = 0;
	ItemInfo.Quantity = 1;
	ItemInfo.Description = TEXT("UNknown");
	ItemInfo.Icon = nullptr;
	ItemInfo.bIsStackable = false;
	ItemInfo.ItemType = EItemType::EIT_Consumable;
	ItemInfo.Actor_Ptr = nullptr;
	ItemInfo.MaxStackNumber = 30;
}

void AItemActor::InitVisible(FBackPackStruct& Message) {
	switch (Message.ItemType) {
	case EItemType::EIT_Consumable:
		Mesh->SetVisibility(true);
		SkeleMesh->SetVisibility(false);
		break;
	case EItemType::EIT_Weapon:
		Mesh->SetVisibility(false);
		SkeleMesh->SetVisibility(true);
		break;
	case EItemType::EIT_Armor:
		Mesh->SetVisibility(true);
		SkeleMesh->SetVisibility(false);
	case EItemType::EIT_Material:
		Mesh->SetVisibility(true);
		SkeleMesh->SetVisibility(false);
	case EItemType::EIT_QuestItem:
		Mesh->SetVisibility(true);
		SkeleMesh->SetVisibility(false);
		break;
	default:
		Mesh->SetVisibility(false);
		SkeleMesh->SetVisibility(false);
		break;
	}
}
