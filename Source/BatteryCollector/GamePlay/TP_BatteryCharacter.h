// Copyright (c) 2014-2019 Sombusta, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TP_BatteryCharacter.generated.h"

UCLASS()
class BATTERYCOLLECTOR_API ATP_BatteryCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BatteryCollector", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* MainCameraArm;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BatteryCollector", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* MainCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BatteryCollector", meta = (AllowPrivateAccess = "true"))
	class USphereComponent* CollectionSphere;
	
public:
	// Sets default values for this character's properties
	ATP_BatteryCharacter();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveForward(float Value);

	void MoveRight(float Value);

	UFUNCTION(BlueprintCallable, Category = "BatteryCollector")
	void CollectPickups();

	UFUNCTION(BlueprintCallable, Category = "BatteryCollector")
	void SetUpdatePower(float PowerChange);

	UFUNCTION(BlueprintImplementableEvent, Category = "BatteryCollector")
	void PowerChangeEffect();

private:
	UPROPERTY(VisibleAnywhere, Category = "Power")
	float CharacterPower;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BatteryCollector", meta = (BlueprintProtected = "true"))
	float InitialPower;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BatteryCollector", meta = (BlueprintProtected = "true"))
	float SpeedFactor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BatteryCollector", meta = (BlueprintProtected = "true"))
	float BaseSpeed;

public:	
	FORCEINLINE class USpringArmComponent* GetMainCameraArm() const { return MainCameraArm; }
	FORCEINLINE class UCameraComponent* GetMainCamera() const { return MainCamera; }
	FORCEINLINE class USphereComponent* GetSphereComponent() const { return CollectionSphere; }

	UFUNCTION(BlueprintPure, Category = "BatteryCollector")
	FORCEINLINE float GetInitialPower() const { return InitialPower; }

	UFUNCTION(BlueprintPure, Category = "BatteryCollector")
	FORCEINLINE float GetCurrentPower() const { return CharacterPower; }
};
