// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DataDefine/AllDataDefine.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/Texture2D.h"
#include "Components/SkeletalMeshComponent.h"
#include "ItemActor.generated.h"

UCLASS()
class AIMTOSHOOT_API AItemActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemActor();

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* Mesh;
	UPROPERTY(VisibleAnywhere)
		USkeletalMeshComponent* SkeleMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Info")
		FBackPackStruct ItemInfo;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Texture2D")
		UTexture2D* ItemTexture2D;

public:
	UFUNCTION(BlueprintCallable, Category = "Initialize Type")
		virtual void InitType();
	UFUNCTION(BlueprintCallable, Category = "Initialize Visible")
		virtual void InitVisible(FBackPackStruct& Message);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;




public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
