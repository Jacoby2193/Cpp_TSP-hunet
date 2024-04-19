// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSPlayerAnim.h"
#include "TPSPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"

void UTPSPlayerAnim::NativeUpdateAnimation( float DeltaSeconds )
{
	// Speed, Direction, IsInAir
	ATPSPlayer* player =  Cast<ATPSPlayer>(TryGetPawnOwner());
	if (player)
	{
		FVector velocity = player->GetVelocity();
		FVector forward = player->GetActorForwardVector();
		FVector right = player->GetActorRightVector();

		Speed = FVector::DotProduct( forward , velocity );
		Direction = FVector::DotProduct( right , velocity );

		IsInAir = player->GetCharacterMovement()->IsFalling();
	}
}
