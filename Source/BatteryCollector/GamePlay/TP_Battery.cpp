// Copyright (c) 2014-2019 Sombusta, All Rights Reserved.


#include "TP_Battery.h"
#include "Components/StaticMeshComponent.h"

ATP_Battery::ATP_Battery()
{
	// PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetSimulatePhysics(true);

	BatteryPower = 150.0f;
}

void ATP_Battery::BeginPlay()
{
	Super::BeginPlay();

}

void ATP_Battery::WasCollected_Implementation()
{
	Super::WasCollected_Implementation();

	Destroy();
}