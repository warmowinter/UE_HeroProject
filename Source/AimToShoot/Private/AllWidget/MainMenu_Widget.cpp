// Fill out your copyright notice in the Description page of Project Settings.


#include "AllWidget/MainMenu_Widget.h"
#include "Components/Button.h"
#include "Components/Border.h"
#include "Kismet/GameplayStatics.h"

void UMainMenu_Widget::NativeConstruct() {
	Super::NativeConstruct();

	if (Button_StartGame) {
		Button_StartGame->OnClicked.AddDynamic(this, &UMainMenu_Widget::OnStartGameClicked);
	}
	if (Button_ContinueGame) {
		Button_ContinueGame->OnClicked.AddDynamic(this, &UMainMenu_Widget::OnContinueGameClicked);
	}
	if (Button_SetChoice) {
		Button_SetChoice->OnClicked.AddDynamic(this, &UMainMenu_Widget::OnSetChoiceClicked);
	}
	if (Button_ExitGame) {
		Button_ExitGame->OnClicked.AddDynamic(this, &UMainMenu_Widget::OnExitGameClicked);
	}
}

void UMainMenu_Widget::OnStartGameClicked()
{
	UE_LOG(LogTemp, Log, TEXT("TestMap Open already"));
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("TestMap"));
	//独立关卡不能作为一个流式关卡。
	//if (Load_WidgetClass) {
	//	UUserWidget* LoadingWidget = CreateWidget<UUserWidget>(GetWorld(), Load_WidgetClass);
	//	if (LoadingWidget) {
	//		LoadingWidget->AddToViewport();
	//	}
	//}

	//FLatentActionInfo LatentInfo;
	//LatentInfo.CallbackTarget = this;

	//UGameplayStatics::LoadStreamLevel(this, "TestMap", true, false, LatentInfo);

}

void UMainMenu_Widget::OnContinueGameClicked()
{
	//todo
}

void UMainMenu_Widget::OnSetChoiceClicked()
{
	//todo
}

void UMainMenu_Widget::OnExitGameClicked()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	UKismetSystemLibrary::QuitGame(GetWorld(), PC, EQuitPreference::Quit, true);

}
