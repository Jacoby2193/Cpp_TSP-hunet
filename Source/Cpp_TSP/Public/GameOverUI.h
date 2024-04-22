// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameOverUI.generated.h"

/**
 * 
 */
UCLASS()
class CPP_TSP_API UGameOverUI : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* ButtonRestart;

	UPROPERTY( EditAnywhere , meta = (BindWidget) )
	class UButton* ButtonQuit;

	UFUNCTION()
	void OnClickRestart();

	UFUNCTION()
	void OnClickQuit();

};
