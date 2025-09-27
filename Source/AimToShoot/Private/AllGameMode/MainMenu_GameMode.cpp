// Fill out your copyright notice in the Description page of Project Settings.


#include "AllGameMode/MainMenu_GameMode.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"

void AMainMenu_GameMode::BeginPlay() {
	Super::BeginPlay();


}


void AMainMenu_GameMode::StartPlay()
{
	Super::StartPlay();


	if (MenuWidgetClass) {
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		if (PlayerController) {
			UUserWidget* MenuWidget = CreateWidget<UUserWidget>(PlayerController, MenuWidgetClass);
			if (MenuWidget) {
				MenuWidget->AddToViewport();

				PlayerController->bShowMouseCursor = true;

				FInputModeUIOnly InputModeDate;

				InputModeDate.SetWidgetToFocus(MenuWidget->TakeWidget());
				PlayerController->SetInputMode(InputModeDate);
				PlayerController->bShowMouseCursor = true;

				//InputModeDate.SetWidgetToFocus(MenuWidget->TakeWidget());
				//InputModeDate.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
				//PlayerController->SetInputMode(InputModeDate);
			}
		}
	}

}
