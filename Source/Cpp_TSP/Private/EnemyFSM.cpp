// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFSM.h"
#include "Enemy.h"
#include "TPSPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "EnemyAnim.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"

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

	EnemyAnim = Cast<UEnemyAnim>(Me->GetMesh()->GetAnimInstance());

	AI = Cast<AAIController>(Me->GetController());

	GetRandomLocationInNavMesh(Me->GetActorLocation(), 500, RandomLocation);
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
	FVector destination = Target->GetActorLocation();
	FVector direction = destination - Me->GetActorLocation();
	//Me->AddMovementInput( direction.GetSafeNormal() );

	// 목적지 경로 탐색
	auto ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());

	FPathFindingQuery query;

	FAIMoveRequest req;

	req.SetAcceptanceRadius(50);
	req.SetGoalLocation(destination);

	AI->BuildPathfindingQuery(req, query);

	auto r = ns->FindPathSync(query);
	if (r.Result == ENavigationQueryResult::Success)
	{
		// 추적
		AI->MoveToLocation( destination , 50 );
	}
	else {
		// 랜덤위치로 이동하는데
		// 만약 도착하거나 그 위치에 문제가 있다면
		// 다시 랜덤위치를 정하고싶다.
		auto patrolResult = AI->MoveToLocation( RandomLocation , 50 );
		if (patrolResult == EPathFollowingRequestResult::AlreadyAtGoal || 
			patrolResult == EPathFollowingRequestResult::Failed)
		{
			GetRandomLocationInNavMesh( Me->GetActorLocation() , 500 , RandomLocation );
		}
	}


	

	// 목적지와의 거리를 구하고
	float dist = direction.Size();
	// 만약 그 거리가 공격 가능거리 이하라면
	if (dist <= AttackRange) {
		// 공격상태로 전이하고싶다.
		SetState( EEnemyState::Attack );
		EnemyAnim->bAttack = true;
	}
}

void UEnemyFSM::TickAttack()
{
	//// 시간이 흐르다가
	//CurrentTime += GetWorld()->DeltaTimeSeconds;
	//// 만약 현재시간이 공격 대기시간보다 커지면
	//if (CurrentTime > AttackDelayTime) {
	//	// 공격을 하고
	//	UE_LOG( LogTemp , Warning , TEXT( "Attack!!!" ) );
	//	// 현재 시간을 초기화 하고싶다.
	//	CurrentTime = 0;
	//}

	//// 목적지와의 거리를 구하고
	//float dist = Me->GetDistanceTo( Target );
	//// 만약 그 거리가 공격 가능거리보다 크다면
	//if (dist > AttackRange) {
	//	// 이동상태로 전이하고싶다.
	//	SetState( EEnemyState::Move );
	//}
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
	if (false == bDieDone)
		return;

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

void UEnemyFSM::OnHit()
{
	// 목적지와의 거리를 구하고
	float dist = Me->GetDistanceTo( Target );
	// 만약 타겟이 공격 가능거리 안에 있으면 공격 시도
	if (dist <= AttackRange)
	{
		UE_LOG( LogTemp , Warning , TEXT( "Attack!!!" ) );
		Target->OnMyTakeDamage();
	}
}

// 공격 동작이 끝나면 
void UEnemyFSM::OnAttackEnd()
{
	// 목적지와의 거리를 구하고
	float dist = Me->GetDistanceTo( Target );
	// 만약 타겟이 공격 가능거리 안에 있으면 공격 시도
	if (dist <= AttackRange)
	{
		EnemyAnim->bAttack = true;
	}
	// 그렇지 않다면
	else
	{
		// 이동상태로 전이하고싶다.
		SetState( EEnemyState::Move );
	}
}

void UEnemyFSM::SetState( EEnemyState newState )
{
	State = newState;
	EnemyAnim->State = newState;
	CurrentTime = 0;
	// 만약 newState가 Move가 아니라면
	// AI를 멈추고싶다.
	if (newState != EEnemyState::Move)
	{
		AI->StopMovement();
	}
}

void UEnemyFSM::OnDamageProcess( int damage )
{
	HP -= damage;

	if (HP > 0) {
		SetState( EEnemyState::Damage );
		// 리액션 애니메이션
		int32 index = FMath::RandRange( 0 , 1 );
		FString sectionName = FString::Printf( TEXT( "Damage%d" ) , index );
		EnemyAnim->PlayDamageAnim(FName(*sectionName));
	}
	else {
		Me->GetCapsuleComponent()->SetCollisionEnabled( ECollisionEnabled::NoCollision );
		SetState( EEnemyState::Die );
		// 죽음 애니메이션
		EnemyAnim->PlayDamageAnim(TEXT("Die"));
		bDieDone = false;
	}
}

bool UEnemyFSM::GetRandomLocationInNavMesh( FVector origin , float radius , FVector& OutLoc )
{
	auto ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());

	FNavLocation loc;
	bool bResult = ns->GetRandomReachablePointInRadius(origin, radius, loc);

	if (bResult)
	{
		OutLoc = loc.Location;
	}

	return bResult;
}

