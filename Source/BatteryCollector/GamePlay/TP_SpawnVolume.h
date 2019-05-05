// Copyright (c) 2014-2019 Sombusta, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TP_SpawnVolume.generated.h"

UCLASS()
class BATTERYCOLLECTOR_API ATP_SpawnVolume : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BatteryCollector", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* WhereToSpawn;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BatteryCollector", meta = (AllowPrivateAccess = "true"))
	class UTextRenderComponent* CountText;

public:	
	// Sets default values for this actor's properties
	ATP_SpawnVolume();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(Blueprintcallable, Category = "BatteryCollector")
	void SetSpawningActive(bool bShouldSpawn);

	void SpawnPickup();

	void AdvanceTimer();

	UFUNCTION(BlueprintPure, Category = "BatteryCollector")
	FVector GetRandomPointInVolume() const;

private:
	float SpawnDelay;

	int32 CountdownTime;

protected:
	UPROPERTY(EditAnywhere, Category = "BatteryCollector", meta = (BlueprintProtected = "true"))
	TSubclassOf<class ATP_PickupBase> WhatToSpawn;

	FTimerHandle SpawnTimer;

	FTimerHandle TextTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "BatteryCollector", meta = (BlueprintProtected = "true"))
	float SpawnDelayRangeLow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BatteryCollector", meta = (BlueprintProtected = "true"))
	float SpawnDelayRangeHigh;

public:	
	FORCEINLINE class UBoxComponent* GetWhereToSpawn() const { return WhereToSpawn; }
};
	