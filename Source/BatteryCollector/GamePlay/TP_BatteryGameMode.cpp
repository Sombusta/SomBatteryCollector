// Copyright (c) 2014-2019 Sombusta, All Rights Reserved.

#include "TP_BatteryGameMode.h"
#include "TP_BatteryCharacter.h"
#include "TP_SpawnVolume.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Blueprint/UserWidget.h"

ATP_BatteryGameMode::ATP_BatteryGameMode()
{
	PrimaryActorTick.bCanEverTick = true;

	DecayRate = 0.01f;
}

void ATP_BatteryGameMode::BeginPlay()
{
	Super::BeginPlay();

	// find all spawn volume actor
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATP_SpawnVolume::StaticClass(), FoundActors);

	for (auto Actor : FoundActors)
	{
		ATP_SpawnVolume* SpawnVolumeActor = Cast<ATP_SpawnVolume>(Actor);

		if (SpawnVolumeActor)
		{
			SpawnVolumeActors.AddUnique(SpawnVolumeActor);
		}
	}

	SetCurrentState(EBatteryPlayState::Playing);

	ATP_BatteryCharacter* MyCharacter = Cast<ATP_BatteryCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));

	if (MyCharacter)
	{
		PowerToWin = (MyCharacter->GetInitialPower()) * 1.25f;
	}

	if (HUDWidgetClass != nullptr)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);

		if (CurrentWidget != nullptr)
		{
			CurrentWidget->AddToViewport();
		}
	}
}

void ATP_BatteryGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ATP_BatteryCharacter* MyCharacter = Cast<ATP_BatteryCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));

	if (MyCharacter)
	{
		if (MyCharacter->GetCurrentPower() > PowerToWin)
		{
			SetCurrentState(EBatteryPlayState::Won);
		}
		else if (MyCharacter->GetCurrentPower() > 0)
		{
			MyCharacter->SetUpdatePower(-DeltaTime * DecayRate * (MyCharacter->GetInitialPower()));
		}
		else
		{
			SetCurrentState(EBatteryPlayState::GameOver);
		}
	}
}

void ATP_BatteryGameMode::HandleNewState(EBatteryPlayState NewState)
{
	switch (NewState)
	{
	case EBatteryPlayState::Playing:
	{
		for (ATP_SpawnVolume* Volume : SpawnVolumeActors)
		{
			Volume->SetSpawningActive(true);
		}
	}
	break;

	case EBatteryPlayState::Won:
	{
		for (ATP_SpawnVolume* Volume : SpawnVolumeActors)
		{
			Volume->SetSpawningActive(false);
		}
	}
	break;

	case EBatteryPlayState::GameOver:
	{
		for (ATP_SpawnVolume* Volume : SpawnVolumeActors)
		{
			Volume->SetSpawningActive(false);
		}

		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);

		if (PlayerController)
		{
			PlayerController->SetCinematicMode(true, false, false, true, true);
		}

		ACharacter* MyCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);

		if (MyCharacter)
		{
			MyCharacter->GetMesh()->SetSimulatePhysics(true);
			MyCharacter->GetMovementComponent()->MovementState.bCanJump = false;
		}
	}
	break;

	default:
	case EBatteryPlayState::Unknown:
	{
		UE_LOG(LogClass, Log, TEXT("UnKnown State"));
	}
	break;
	}
}

void ATP_BatteryGameMode::SetCurrentState(EBatteryPlayState NewState)
{
	CurrentState = NewState;
	HandleNewState(CurrentState);
}