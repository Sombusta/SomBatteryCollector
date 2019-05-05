// Copyright (c) 2014-2019 Sombusta, All Rights Reserved.

#include "TP_PickupBase.h"
#include "Components/StaticMeshComponent.h"
#include "Engine.h"

// Sets default values
ATP_PickupBase::ATP_PickupBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));

	RootComponent = PickupMesh;

	bIsActive = true;
}

// Called when the game starts or when spawned
void ATP_PickupBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATP_PickupBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATP_PickupBase::SetActive(bool NewPickupState)
{
	bIsActive = NewPickupState;
}

void ATP_PickupBase::WasCollected_Implementation()
{
	FString PickupDebugString = GetName();

	UE_LOG(LogTemp, Log, TEXT("You have collected %s"), *PickupDebugString);

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, "You have collected " + PickupDebugString);
}