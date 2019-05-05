// Copyright (c) 2014-2019 Sombusta, All Rights Reserved.

#include "TP_BatteryCharacter.h"
#include "TP_PickupBase.h"
#include "TP_Battery.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/InputComponent.h"

#include "Engine.h"

// Sets default values
ATP_BatteryCharacter::ATP_BatteryCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MainCameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("MainCameraArm"));
	MainCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("MainCamera"));
	CollectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollectionSphere"));
		
	MainCameraArm->SetupAttachment(RootComponent);
	MainCameraArm->TargetArmLength = 300.0f;
	MainCameraArm->bUsePawnControlRotation = true;
	
	MainCamera->SetupAttachment(MainCameraArm);
	MainCamera->bUsePawnControlRotation = false;

	CollectionSphere->SetupAttachment(RootComponent);
	CollectionSphere->SetSphereRadius(200.0f);

	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);

	bUseControllerRotationRoll = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 600.0f;
	GetCharacterMovement()->AirControl = 0.2f;

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -93.0f), FRotator(0.0f, -90.f, 0.0f));
	
	InitialPower = 2000.0f;
	CharacterPower = InitialPower;

	SpeedFactor = 0.75f;
	BaseSpeed = 10.0f;
}

// Called when the game starts or when spawned
void ATP_BatteryCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	CharacterPower = InitialPower;
}

// Called every frame
void ATP_BatteryCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATP_BatteryCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &ATP_BatteryCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ATP_BatteryCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindKey(EKeys::C, IE_Pressed, this, &ATP_BatteryCharacter::CollectPickups);
}

void ATP_BatteryCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ATP_BatteryCharacter::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

void ATP_BatteryCharacter::SetUpdatePower(float PowerChange)
{
	CharacterPower += PowerChange;

	GetCharacterMovement()->MaxWalkSpeed = BaseSpeed + (SpeedFactor * CharacterPower);

	PowerChangeEffect();
}

void ATP_BatteryCharacter::CollectPickups()
{
	UE_LOG(LogClass, Log, TEXT("Collected!!"));

	// SomWorks :D // Get all overlapping Actors and store them in an array
	TArray<AActor*> CollectedActors;
	CollectionSphere->GetOverlappingActors(CollectedActors);

	// keep track of the collected battery power
	float CollectedPower = 0.0f;

	// SomWorks :D // For each Actor we collected
	for (int32 iCollected = 0; iCollected < CollectedActors.Num(); iCollected++)
	{
		// SomWorks :D // Cast the actor to APickup
		ATP_PickupBase* const TestPickup = Cast<ATP_PickupBase>(CollectedActors[iCollected]);

		// SomWorks :D // If the cast is successful and the pickup is valid and active
		if (TestPickup && !TestPickup->IsPendingKill() && TestPickup->IsActive())
		{
			// SomWorks :D // call the pickup's WasCollected function
			TestPickup->WasCollected();

			// SomWorks :D //
			ATP_Battery* const TestBattery = Cast<ATP_Battery>(TestPickup);

			if (TestBattery)
			{
				CollectedPower += TestBattery->GetPower();
			}

			// SomWorks :D // Deactivate the pickup
			TestPickup->SetActive(false);
		}
	}

	if (CollectedPower > 0)
	{
		SetUpdatePower(CollectedPower);
	}
}