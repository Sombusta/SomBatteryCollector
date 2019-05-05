// Copyright (c) 2014-2019 Sombusta, All Rights Reserved.

#include "TP_SpawnVolume.h"
#include "TP_PickupBase.h"
#include "Components/BoxComponent.h"
#include "Components/TextRenderComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/World.h"
#include "TimerManager.h"

// Sets default values
ATP_SpawnVolume::ATP_SpawnVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WhereToSpawn = CreateDefaultSubobject<UBoxComponent>(TEXT("WhereToSpawn"));
	CountText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("CountText"));

	RootComponent = WhereToSpawn;
	
	WhereToSpawn->SetCollisionProfileName("NoCollision");

	CountText->SetupAttachment(RootComponent);
	CountText->SetWorldSize(50.0f);
	CountText->SetHorizontalAlignment(EHTA_Center);
	CountText->SetVerticalAlignment(EVRTA_TextCenter);
	CountText->SetText(TEXT("0"));

	SpawnDelayRangeLow = 1.0f;
	SpawnDelayRangeHigh = 4.5f;
}

// Called when the game starts or when spawned
void ATP_SpawnVolume::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATP_SpawnVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATP_SpawnVolume::SetSpawningActive(bool bShouldSpawn)
{
	if (bShouldSpawn)
	{
		SpawnDelay = FMath::FRandRange(SpawnDelayRangeLow, SpawnDelayRangeHigh);
		CountdownTime = SpawnDelay;
		
		GetWorldTimerManager().SetTimer(SpawnTimer, this, &ATP_SpawnVolume::SpawnPickup, SpawnDelay, false);
		GetWorldTimerManager().SetTimer(TextTimer, this, &ATP_SpawnVolume::AdvanceTimer, 1.0f, true);
	}
	else
	{
		GetWorldTimerManager().ClearTimer(SpawnTimer);
		GetWorldTimerManager().ClearTimer(TextTimer);
	}
}

void ATP_SpawnVolume::SpawnPickup()
{
	if (WhatToSpawn != nullptr)
	{
		UWorld* const World = GetWorld();

		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = Instigator;

			FVector SpawnLocation = GetRandomPointInVolume();

			FRotator SpawnRotation;
			SpawnRotation.Yaw = FMath::FRand() * 360.0f;
			SpawnRotation.Pitch = FMath::FRand() * 360.0f;
			SpawnRotation.Roll = FMath::FRand() * 360.0f;

			ATP_PickupBase* const SpawnedPickup = World->SpawnActor<ATP_PickupBase>(WhatToSpawn, SpawnLocation, SpawnRotation, SpawnParams);

			SpawnDelay = FMath::FRandRange(SpawnDelayRangeLow, SpawnDelayRangeHigh);
			CountdownTime = SpawnDelay;

			GetWorldTimerManager().SetTimer(SpawnTimer, this, &ATP_SpawnVolume::SpawnPickup, SpawnDelay, false);
			GetWorldTimerManager().SetTimer(TextTimer, this, &ATP_SpawnVolume::AdvanceTimer, 1.0f, true);
		}
	}
}

void ATP_SpawnVolume::AdvanceTimer()
{
	--CountdownTime;

	CountText->SetText(FText::FromString(FString::FromInt(FMath::Max(CountdownTime, 0))));

	if (CountdownTime < 1)
	{
		// We're done counting down, so stop running the timer.
		GetWorldTimerManager().ClearTimer(TextTimer);
	}
}

FVector ATP_SpawnVolume::GetRandomPointInVolume() const
{
	FVector SpawnOrigin = WhereToSpawn->Bounds.Origin;
	FVector SpawnExtent = WhereToSpawn->Bounds.BoxExtent;

	return UKismetMathLibrary::RandomPointInBoundingBox(SpawnOrigin, SpawnExtent);
}