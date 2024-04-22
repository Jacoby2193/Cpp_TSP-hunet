// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHPUI.generated.h"

/**
 * 
 */
UCLASS()
class CPP_TSP_API UPlayerHPUI : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	void SetHP( int32 curHP , int32 maxHP );

	// Bar두개를 바인드
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UProgressBar* HPBarBack;

	UPROPERTY( EditAnywhere , meta = (BindWidget) )
	class UProgressBar* HPBar;
};
