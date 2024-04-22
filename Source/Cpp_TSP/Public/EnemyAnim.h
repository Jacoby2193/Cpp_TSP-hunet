// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyFSM.h"
#include "EnemyAnim.generated.h"

/**
 * 
 */
UCLASS()
class CPP_TSP_API UEnemyAnim : public UAnimInstance
{
	GENERATED_BODY()

public:
	// 상태
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EEnemyState State;

	UPROPERTY( EditDefaultsOnly , BlueprintReadOnly )
	bool bAttack;

	UFUNCTION(BlueprintCallable)
	void OnHit();

	UFUNCTION(BlueprintCallable)
	void OnAttackEnd();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayDamageAnim(FName sectionName);

	UFUNCTION()
	void AnimNotify_DieEnd();

};
