// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSPlayer.h"
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include <Engine/SkeletalMesh.h>
#include <../../../../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h>
#include <../../../../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Blueprint/UserWidget.h>
#include "BulletActor.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "Enemy.h"

// Sets default values
ATPSPlayer::ATPSPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 200;
	SpringArmComp->SetRelativeLocation(FVector(0, 40, 70));

	CamComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CamComp"));
	CamComp->SetupAttachment(SpringArmComp);

	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Quinn.SKM_Quinn'"));

	if (TempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}

	GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMesh"));
	GunMesh->SetCollisionProfileName(TEXT("NoCollision"));
	GunMesh->SetupAttachment(GetMesh());
	
	SniperMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SniperMesh"));;
	SniperMesh->SetCollisionProfileName(TEXT("NoCollision"));
	SniperMesh->SetupAttachment(GetMesh());

	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempGunMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Models/FPWeapon/Mesh/SK_FPGun.SK_FPGun'"));
	if (TempGunMesh.Succeeded())
	{
		GunMesh->SetSkeletalMesh(TempGunMesh.Object);
	}

	ConstructorHelpers::FObjectFinder<UStaticMesh> TempSniperMesh(TEXT("/Script/Engine.StaticMesh'/Game/Models/SniperGun/sniper1.sniper1'"));
	if (TempSniperMesh.Succeeded())
	{
		SniperMesh->SetStaticMesh(TempSniperMesh.Object);
	}
}

// Called when the game starts or when spawned
void ATPSPlayer::BeginPlay()
{
	Super::BeginPlay();

	auto pc = Cast<APlayerController>(GetController());
	if (pc)
	{
		auto subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());
		if (subsystem)
		{
			subsystem->ClearAllMappings();
			subsystem->AddMappingContext(IMC_Player, 0);
		}
	}

	// Crosshair와 Sniper 위젯을 생성해서 기억하고싶다.

	CrosshairUI = CreateWidget(GetWorld(), CrosshairUIFactory);
	SniperUI = CreateWidget(GetWorld(), SniperUIFactory);

	CrosshairUI->AddToViewport();
	SniperUI->AddToViewport();

	// 태어날 때 기본총으로 보이게 하고싶다.
	OnIAGun(FInputActionValue());

}

// Called every frame
void ATPSPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 절대적인 방향을 나를 기준으로 회전하고싶다.
	Direction = FTransform(GetControlRotation()).TransformVector(Direction);
	AddMovementInput(Direction);

	Direction = FVector::ZeroVector;

}

// Called to bind functionality to input
void ATPSPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	auto* Input = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	Input->BindAction(IA_Move, ETriggerEvent::Triggered, this, &ATPSPlayer::OnIAMove);
	Input->BindAction(IA_Look, ETriggerEvent::Triggered, this, &ATPSPlayer::OnIALook);
	Input->BindAction(IA_Jump, ETriggerEvent::Started, this, &ATPSPlayer::OnIAJump);
	
	Input->BindAction(IA_Gun, ETriggerEvent::Started, this, &ATPSPlayer::OnIAGun);
	Input->BindAction(IA_Sniper, ETriggerEvent::Started, this, &ATPSPlayer::OnIASniper);
	Input->BindAction(IA_Zoom, ETriggerEvent::Started, this, &ATPSPlayer::OnIAZoomIn);
	Input->BindAction(IA_Zoom, ETriggerEvent::Completed, this, &ATPSPlayer::OnIAZoomOut);
	Input->BindAction(IA_Fire, ETriggerEvent::Started, this, &ATPSPlayer::OnIAFire);

}

void ATPSPlayer::OnIAMove(const FInputActionValue& value)
{
	auto vec = value.Get<FVector2D>();
	Direction.X = vec.X;
	Direction.Y = vec.Y;
}

void ATPSPlayer::OnIALook(const FInputActionValue& value)
{
	auto vec = value.Get<FVector2D>();
	AddControllerPitchInput(vec.Y);
	AddControllerYawInput(vec.X);
}

void ATPSPlayer::OnIAJump(const FInputActionValue& value)
{
	Jump();
}

void ATPSPlayer::OnIAGun(const FInputActionValue& value)
{
	bChooseSniperGun = false;
	// 유탄총이 보이고, 스나이퍼가 안보이게
	GunMesh->SetVisibility(true);
	SniperMesh->SetVisibility(false);

	// UI를 보이지 않게 하고싶다.
	CrosshairUI->SetVisibility(ESlateVisibility::Hidden);
	SniperUI->SetVisibility(ESlateVisibility::Hidden);
}

void ATPSPlayer::OnIASniper(const FInputActionValue& value)
{
	bChooseSniperGun = true;
	// 유탄총이 안보이고, 스나이퍼가 보이게
	GunMesh->SetVisibility(false);
	SniperMesh->SetVisibility(true);

	// CrosshairUI만 보이게 하고싶다.
	CrosshairUI->SetVisibility(ESlateVisibility::Visible);
	SniperUI->SetVisibility(ESlateVisibility::Hidden);

}

void ATPSPlayer::OnIAZoomIn(const FInputActionValue& value)
{
	if (false == bChooseSniperGun)
		return;

	// SniperUI만 보이게 하고싶다.
	CrosshairUI->SetVisibility(ESlateVisibility::Hidden);
	SniperUI->SetVisibility(ESlateVisibility::Visible);

	CamComp->FieldOfView = 20;

}

void ATPSPlayer::OnIAZoomOut(const FInputActionValue& value)
{
	if (false == bChooseSniperGun)
		return;

	// CrosshairUI만 보이게 하고싶다.
	CrosshairUI->SetVisibility(ESlateVisibility::Visible);
	SniperUI->SetVisibility(ESlateVisibility::Hidden);

	CamComp->FieldOfView = 90;
}

void ATPSPlayer::OnIAFire(const FInputActionValue& value)
{
	UE_LOG(LogTemp, Warning, TEXT("OnIAFire"));
	if (bChooseSniperGun)
	{
		// 스나이퍼
		FHitResult hitInfo;
		FVector start = CamComp->GetComponentLocation();
		FVector end = start + CamComp->GetForwardVector() * 100000;
		FCollisionQueryParams params;
		params.AddIgnoredActor(this);
		bool bHit = GetWorld()->LineTraceSingleByChannel(hitInfo, start, end, ECC_Visibility, params);
		if (bHit)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionVFXFactory, hitInfo.ImpactPoint);

			// 부딪힌 상대방이 AEnemy라면 적에게 데미지를 1점 주고싶다.
			// AEnemy의 FSM의 OnDamageProcess(1);
			auto* enemy = Cast<AEnemy>(hitInfo.GetActor());
			if (enemy)
			{
				enemy->OnMyTakeDamage(1);
			}
		}

	}
	else
	{
		// 기본총
		FTransform t = GunMesh->GetSocketTransform(TEXT("FirePosition"));
		GetWorld()->SpawnActor<ABulletActor>(BulletFactory, t);
	}
}

