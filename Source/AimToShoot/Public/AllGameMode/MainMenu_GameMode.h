// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainMenu_GameMode.generated.h"

/**
 * 
 */

class UUserWidget;

UCLASS()
class AIMTOSHOOT_API AMainMenu_GameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	virtual void StartPlay() override;


	//选择指定的UI
	UPROPERTY(EditAnywhere, Category = "UI")
		TSubclassOf<UUserWidget> MenuWidgetClass;

};
