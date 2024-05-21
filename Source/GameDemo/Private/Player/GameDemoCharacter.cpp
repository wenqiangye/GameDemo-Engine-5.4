// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/GameDemoCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameDemoPlayerController.h"
#include "Components/GMCharacterMovementComponent.h"
#include "Components/GDHealthyComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/GDWeaponComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/DamageEvents.h"

DEFINE_LOG_CATEGORY_STATIC(LogGameBaseCharacter, All, All)

// Sets default values
AGameDemoCharacter::AGameDemoCharacter(const FObjectInitializer& ObjInit) :
	Super(ObjInit.SetDefaultSubobjectClass<UGMCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->SocketOffset = FVector(0.0f, 100.0f, 0);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);

	HealthComponent = CreateDefaultSubobject<UGDHealthyComponent>("HealthComponent");

	HealthTextComponent = CreateDefaultSubobject<UTextRenderComponent>("HealthTextComponent");
	HealthTextComponent->SetupAttachment(GetRootComponent());
	HealthTextComponent->SetOwnerNoSee(true);

	WeaponComponent = CreateDefaultSubobject<UGDWeaponComponent>("WeaponComponent");
}

// Called when the game starts or when spawned
void AGameDemoCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* GameDemoPlayerController =
		CastChecked<AGameDemoPlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
				GameDemoPlayerController->GetLocalPlayer()))
		{
			UE_LOG(LogGameBaseCharacter, Warning, TEXT("EnhancedInput Available"));
			Subsystem->AddMappingContext(InputMappingContext, 100);
		}
	}
	OnHealthChanged(HealthComponent->GetHealth());
	HealthComponent->OnDeath.AddUObject(this, &AGameDemoCharacter::OnDeath);
	HealthComponent->OnHealthChanged.AddUObject(this, &AGameDemoCharacter::OnHealthChanged);

	LandedDelegate.AddDynamic(this, &AGameDemoCharacter::OnGroundLanded);
}

// Called every frame
void AGameDemoCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGameDemoCharacter::MoveForward(const FInputActionValue& Value)
{
	IsMovingForward = Value.Get<float>() > 0.0f;
	FVector2D MovementVector = Value.Get<FVector2D>();
	if (Controller != nullptr)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector ForwardDirection =
			FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(ForwardDirection, MovementVector.X);
		// UE_LOG(LogGameBaseCharacter, Warning, TEXT("Move forward"));
	}
}

void AGameDemoCharacter::MoveRight(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	if (Controller != nullptr)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector RightDirection =
			FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AGameDemoCharacter::TurnAtRate(const FInputActionValue& Value)
{
	FVector2D TurnAtVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(TurnAtVector.X);
	}
}

void AGameDemoCharacter::LookUpAtRate(const FInputActionValue& Value)
{
	FVector2D LookUpVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerPitchInput(LookUpVector.Y);
		AddControllerYawInput(LookUpVector.X);
	}
}

void AGameDemoCharacter::OnStartRunning()
{
	WantsToRun = true;
}

void AGameDemoCharacter::OnStopRunning()
{
	WantsToRun = false;
}

void AGameDemoCharacter::OnDeath()
{
	// UE_LOG(LogGameBaseCharacter, Display, TEXT("Player %s is dead"), *GetName());
	PlayAnimMontage(DeathAnimeMontage);

	GetCharacterMovement()->DisableMovement();
	SetLifeSpan(LifeSpanDeath);

	if (Controller)
	{
		Controller->ChangeState(NAME_Spectating);
	}
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
}

void AGameDemoCharacter::OnHealthChanged(float Health)
{
	HealthTextComponent->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), Health)));
}

void AGameDemoCharacter::OnGroundLanded(const FHitResult& Hit)
{
	const auto FallVelocityZ = -GetVelocity().Z;
	// UE_LOG(LogGameBaseCharacter, Display, TEXT("Player landed: %f"), FallVelocityZ)

	if (FallVelocityZ < LandedDamageVelocity.X) return;
	const auto FinalDamage = FMath::GetMappedRangeValueClamped(LandedDamageVelocity, LandedDamage, FallVelocityZ);
	// UE_LOG(LogGameBaseCharacter, Display, TEXT("FinalDamage : %f"), FinalDamage)
	TakeDamage(FinalDamage, FDamageEvent{}, nullptr, nullptr);
}


// Called to bind functionality to input
void AGameDemoCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);
	check(WeaponComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent =
		CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (IA_MoveForward)
		{
			EnhancedInputComponent->BindAction(IA_MoveForward,
			                                   ETriggerEvent::Triggered, this,
			                                   &AGameDemoCharacter::MoveForward);
		}

		if (IA_MoveRight)
		{
			EnhancedInputComponent->BindAction(IA_MoveRight, ETriggerEvent::Triggered,
			                                   this, &AGameDemoCharacter::MoveRight);
		}

		if (IA_Turn)
		{
			EnhancedInputComponent->BindAction(IA_Turn, ETriggerEvent::Triggered,
			                                   this, &AGameDemoCharacter::TurnAtRate);
		}

		if (IA_LookUp)
		{
			EnhancedInputComponent->BindAction(IA_LookUp, ETriggerEvent::Triggered,
			                                   this, &AGameDemoCharacter::LookUpAtRate);
		}

		if (IA_Jump)
		{
			EnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Started, this,
			                                   &ACharacter::Jump);
			EnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Completed,
			                                   this, &ACharacter::StopJumping);
		}

		if (IA_Run)
		{
			EnhancedInputComponent->BindAction(IA_Run, ETriggerEvent::Started, this,
			                                   &AGameDemoCharacter::OnStartRunning);
			EnhancedInputComponent->BindAction(IA_Run, ETriggerEvent::Completed, this,
			                                   &AGameDemoCharacter::OnStopRunning);
		}

		if (IA_Fire)
		{
			EnhancedInputComponent->BindAction(IA_Fire, ETriggerEvent::Started, WeaponComponent,
			                                   &UGDWeaponComponent::StartFire);
			EnhancedInputComponent->BindAction(IA_Fire, ETriggerEvent::Completed, WeaponComponent,
			                                   &UGDWeaponComponent::StopFire);
		}
		if (IA_NextWeapon)
		{
			EnhancedInputComponent->BindAction(IA_NextWeapon, ETriggerEvent::Triggered,
			                                   WeaponComponent, &UGDWeaponComponent::NextWeapon);
		}
	}
}

bool AGameDemoCharacter::IsRunining() const
{
	return WantsToRun && IsMovingForward && !GetVelocity().IsZero();
}

float AGameDemoCharacter::GetMovementDirection() const
{
	const auto VelocityNormal = GetVelocity().GetSafeNormal();
	const auto AngleBetween = FMath::Acos(FVector::DotProduct(GetActorForwardVector(), VelocityNormal));
	const auto CrossProduct = FVector::CrossProduct(GetActorForwardVector(), VelocityNormal);
	return FMath::RadiansToDegrees(AngleBetween) * FMath::Sign(CrossProduct.Z);
}

FString AGameDemoCharacter::GetWeaponType() const
{
	if (!WeaponComponent) return "No_Weapon";
	return WeaponComponent->GetWeaponType();
}

bool AGameDemoCharacter::IsAttack() const
{
	return WantToAttack;
}
