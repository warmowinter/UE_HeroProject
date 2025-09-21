// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenu_Widget.generated.h"

/**
 * 
 */

class UBorder;
class UButton;

UCLASS()
class AIMTOSHOOT_API UMainMenu_Widget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Meta = (BindWidget))
		UButton* Button_StartGame;
	UPROPERTY(Meta = (BindWidget))
		UButton* Button_ContinueGame;
	UPROPERTY(Meta = (BindWidget))
		UButton* Button_SetChoice;
	UPROPERTY(Meta = (BindWidget))
		UButton* Button_ExitGame;



};
