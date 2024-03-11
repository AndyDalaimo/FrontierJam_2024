// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ShopDayCycle.generated.h"


// Game State
UENUM(BlueprintType)
enum class EGameState : uint8
{
	DAY = 0 UMETA(DisplayName = "DayCycle"),
	NIGHT = 1 UMETA(DisplayName = "NightCycle"),
};



/**
 * 
 */
UCLASS()
class FRONTIERJAM_API UShopDayCycle : public UGameInstance
{
	GENERATED_BODY()

		UShopDayCycle(const FObjectInitializer& ObjectInitializer);
	
public:


	UFUNCTION(BlueprintCallable)
		void InitializeDayCycle();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
		EGameState GameState;

private:

	// Properties for in game timer
	FTimerHandle DayTimer;
	FTimerDelegate TimerDelegate;

	// In Game Time for day cycle
	UPROPERTY(EditDefaultsOnly)
		int32 dayCycleTime;

	UPROPERTY(EditDefaultsOnly)
	float timerRate = 1.f;

	float timeElapsed = 0.f;

	// The timer function called during the day cycle timer
	UFUNCTION(CallInEditor)
	void DayCycle();

};
