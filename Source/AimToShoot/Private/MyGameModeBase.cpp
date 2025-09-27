// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameModeBase.h"
#include "Hero.h" 
#include "GameFramework/PlayerController.h"


AMyGameModeBase::AMyGameModeBase() {

}


void AMyGameModeBase::BeginPlay() {
	Super::BeginPlay();



}

void AMyGameModeBase::StartPlay() {
	Super::StartPlay();

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

	if (PlayerController) {
		FInputModeGameOnly InputMode;
		PlayerController->SetInputMode(InputMode);
		PlayerController->bShowMouseCursor = false;
	}


}
