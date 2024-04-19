// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFSM.h"
#include "Enemy.h"
#include "TPSPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"

// Sets default values for this component's properties
UEnemyFSM::UEnemyFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEnemyFSM::BeginPlay()
{
	Super::BeginPlay();

	Me = Cast<AEnemy>( GetOwner() );

}


// Called every frame
void UEnemyFSM::TickComponent( float DeltaTime , ELevelTick TickType , FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime , TickType , ThisTickFunction );

	switch (State)
	{
	case EEnemyState::Idle:
		TickIdle();
		break;
	case EEnemyState::Move:
		TickMove();
		break;
	case EEnemyState::Attack:
		TickAttack();
		break;
	case EEnemyState::Damage:
		TickDamage();
		break;
	case EEnemyState::Die:
		TickDie();
		break;
	}

	// 현재 상태를 화면에 로그로 출력하고싶다.
	FString strState = UEnum::GetValueAsString<EEnemyState>( State );
	DrawDebugString( GetWorld() , Me->GetActorLocation() + FVector( 0 , 0 , 50 ) , strState , 0 , FColor::Cyan , 0 , true );
}

void UEnemyFSM::TickIdle()
{
	// 시간이 흐르다가
	CurrentTime += GetWorld()->DeltaTimeSeconds;
	// 만약 FindTime보다 커졌다면
	if (CurrentTime > FindTime)
	{
		CurrentTime = 0;
		// 목적지(주인공)을 찾고싶다.
		Target = Cast<ATPSPlayer>( UGameplayStatics::GetActorOfClass( GetWorld() , ATPSPlayer::StaticClass() ) );
		// 만약 찾았다면 Move상태로 전이하고싶다.
		if (Target)
		{
			SetState( EEnemyState::Move );
		}

	}
}

void UEnemyFSM::TickMove()
{
	// 목적지를 향해 이동하고싶다.
	FVector direction = Target->GetActorLocation() - Me->GetActorLocation();
	Me->AddMovementInput( direction.GetSafeNormal() );


	// 목적지와의 거리를 구하고
	float dist = direction.Size();
	// 만약 그 거리가 공격 가능거리 이하라면
	if (dist <= AttackRange) {
		// 공격상태로 전이하고싶다.
		SetState( EEnemyState::Attack );
	}
}

void UEnemyFSM::TickAttack()
{
	// 시간이 흐르다가
	CurrentTime += GetWorld()->DeltaTimeSeconds;
	// 만약 현재시간이 공격 대기시간보다 커지면
	if (CurrentTime > AttackDelayTime) {
		// 공격을 하고
		UE_LOG( LogTemp , Warning , TEXT( "Attack!!!" ) );
		// 현재 시간을 초기화 하고싶다.
		CurrentTime = 0;
	}

	// 목적지와의 거리를 구하고
	float dist = Me->GetDistanceTo( Target );
	// 만약 그 거리가 공격 가능거리보다 크다면
	if (dist > AttackRange) {
		// 이동상태로 전이하고싶다.
		SetState( EEnemyState::Move );
	}
}

void UEnemyFSM::TickDamage()
{
	// 시간이 흐르다가
	CurrentTime += GetWorld()->DeltaTimeSeconds;
	// 만약 현재시간이 데미지딜레이 시간이 되면
	if (CurrentTime > DamageDelayTime)
	{
		// 이동상태로 전이하고싶다.
		SetState( EEnemyState::Move );
	}
}

void UEnemyFSM::TickDie()
{
	// 아래방향으로 이동하고싶다.
	// P = P0 + vt
	FVector P0 = Me->GetActorLocation();
	FVector vt = FVector::DownVector * 200 * GetWorld()->DeltaTimeSeconds;
	Me->SetActorLocation( P0 + vt );

	// 시간이 흐르다가
	CurrentTime += GetWorld()->DeltaTimeSeconds;
	// 만약 현재시간이 죽음딜레이 시간이 되면
	if (CurrentTime > DieDelayTime)
	{
		// Me를 파괴하고싶다.
		Me->Destroy();
	}
}

void UEnemyFSM::SetState( EEnemyState newState )
{
	State = newState;
	CurrentTime = 0;
}

void UEnemyFSM::OnDamageProcess( int damage )
{
	HP -= damage;

	if (HP > 0) {
		SetState( EEnemyState::Damage );
	}
	else {
		Me->GetCapsuleComponent()->SetCollisionEnabled( ECollisionEnabled::NoCollision );
		SetState( EEnemyState::Die );
	}
}

