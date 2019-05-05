// Copyright (c) 2014-2019 Sombusta, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TP_BatteryGameMode.generated.h"

UENUM(BlueprintType)
enum class EBatteryPlayState : uint8
{
	Playing = 0,
	GameOver,
	Won,
	Unknown
};

UCLASS()
class BATTERYCOLLECTOR_API ATP_BatteryGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ATP_BatteryGameMode();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

private:
	void HandleNewState(EBatteryPlayState NewState);

	void SetCurrentState(EBatteryPlayState NewState);
	
	EBatteryPlayState CurrentState;

	TArray<class ATP_SpawnVolume*> SpawnVolumeActors;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "BatteryCollector", meta = (BlueprintProtected = "true"))
	float DecayRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "BatteryCollector", meta = (BlueprintProtected = "true"))
	float PowerToWin;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "BatteryCollector", meta = (BlueprintProtected = "true"))
	TSubclassOf<class UUserWidget> HUDWidgetClass;

	UPROPERTY()
	class UUserWidget* CurrentWidget;

public:
	UFUNCTION(BlueprintPure, Category = "BatteryCollector")
	FORCEINLINE float GetPowerToWin() const { return PowerToWin; }

	UFUNCTION(BlueprintPure, Category = "BatteryCollector")
	FORCEINLINE EBatteryPlayState GetCurrentState() const { return CurrentState; }
};