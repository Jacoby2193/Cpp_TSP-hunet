// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHPUI.h"
#include "Components/ProgressBar.h"

void UPlayerHPUI::NativeTick( const FGeometry& MyGeometry , float InDeltaTime )
{
	float A = HPBarBack->GetPercent();
	float B = HPBar->GetPercent();
	A = FMath::Lerp(A, B, InDeltaTime * 6);
	HPBarBack->SetPercent(A);
}

void UPlayerHPUI::SetHP( int32 curHP , int32 maxHP )
{
	float per = (float)curHP / maxHP;
	HPBar->SetPercent(per);
}
