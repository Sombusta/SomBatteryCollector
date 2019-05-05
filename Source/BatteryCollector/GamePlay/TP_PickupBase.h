// Copyright (c) 2014-2019 Sombusta, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TP_PickupBase.generated.h"

UCLASS()
class BATTERYCOLLECTOR_API ATP_PickupBase : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BatteryCollector", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* PickupMesh;
	
public:	
	// Sets default values for this actor's properties
	ATP_PickupBase();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "BatteryCollector")
	void SetActive(bool NewPickupState);

	UFUNCTION(BlueprintNativeEvent, Category = "BatteryCollector")
	void WasCollected();
	virtual void WasCollected_Implementation();
	
private:

protected:
	bool bIsActive = false;

public:	
	FORCEINLINE class UStaticMeshComponent* GetMesh() const { return PickupMesh; }
	FORCEINLINE bool GetActive() const { return bIsActive; }

	UFUNCTION(BlueprintPure, Category = "BatteryCollector")
	FORCEINLINE bool IsActive() const { return bIsActive; }
};
