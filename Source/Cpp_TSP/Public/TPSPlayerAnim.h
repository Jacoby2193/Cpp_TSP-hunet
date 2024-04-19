// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "TPSPlayerAnim.generated.h"

/**
 * 
 */
UCLASS()
class CPP_TSP_API UTPSPlayerAnim : public UAnimInstance
{
	GENERATED_BODY()
public:
	 virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
	 UPROPERTY(EditAnywhere, BlueprintReadWrite)
	 float Speed;

	 UPROPERTY( EditAnywhere , BlueprintReadWrite )
	 float Direction;

	 UPROPERTY( EditAnywhere , BlueprintReadWrite )
	 bool IsInAir;

};
