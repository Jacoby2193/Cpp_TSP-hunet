// Fill out your copyright notice in the Description page of Project Settings.


#include "GameOverUI.h"
#include "Components/Button.h"
#include <Kismet/KismetSystemLibrary.h>
#include <Kismet/GameplayStatics.h>

void UGameOverUI::NativeConstruct()
{
	// 버튼에 함수를 연결하고싶다.
	ButtonRestart->OnClicked.AddDynamic( this , &UGameOverUI::OnClickRestart );
	ButtonQuit->OnClicked.AddDynamic( this , &UGameOverUI::OnClickQuit );
}

void UGameOverUI::OnClickRestart()
{
	FString levelName = UGameplayStatics::GetCurrentLevelName( GetWorld() );
	UGameplayStatics::OpenLevel( GetWorld() , FName(*levelName));
}

void UGameOverUI::OnClickQuit()
{
	UKismetSystemLibrary::QuitGame( GetWorld() , GetWorld()->GetFirstPlayerController() , EQuitPreference::Quit , false );
}
