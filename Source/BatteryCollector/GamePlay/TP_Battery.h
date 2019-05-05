// Copyright (c) 2014-2019 Sombusta, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GamePlay/TP_PickupBase.h"
#include "TP_Battery.generated.h"

/**
 * 
 */
UCLASS()
class BATTERYCOLLECTOR_API ATP_Battery : public ATP_PickupBase
{
	GENERATED_BODY()
	
public:
	ATP_Battery();

	virtual void BeginPlay() override;

	virtual void WasCollected_Implementation() override;

private:

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BatteryCollector", meta = (BlueprintProtected = "true"))
	float BatteryPower;

public:
	UFUNCTION(BlueprintPure, Category = "BatteryCollector")
	FORCEINLINE float GetPower() const { return BatteryPower; }

};
