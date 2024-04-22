// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnim.h"
#include "Enemy.h"
#include "EnemyFSM.h"

void UEnemyAnim::OnHit()
{
	// FSM의 OnHit()을 호출하고싶다.
	auto* enemy = Cast<AEnemy>( TryGetPawnOwner() );
	if (enemy && enemy->FSM)
	{
		enemy->FSM->OnHit();
	}
}

void UEnemyAnim::OnAttackEnd()
{
	// FSM의 OnAttackEnd()을 호출하고싶다.
	auto* enemy = Cast<AEnemy>( TryGetPawnOwner() );
	if (enemy && enemy->FSM)
	{
		enemy->FSM->OnAttackEnd();
	}
}

void UEnemyAnim::AnimNotify_DieEnd()
{
	// FSM의 bDieDone = true;
	auto* enemy = Cast<AEnemy>( TryGetPawnOwner() );
	if (enemy && enemy->FSM)
	{
		enemy->FSM->bDieDone = true;
	}
}
