// Fill out your copyright notice in the Description page of Project Settings.

#include "ShopDayCycle.h"

#include "TimerManager.h"



UShopDayCycle::UShopDayCycle(const FObjectInitializer& ObjectInitializer) : dayCycleTime(10), timerRate(1.f), GameState(EGameState::DAY)
{

}


// Start Day Cycle
// Player will interact with world during this cycle. Will Restart when player decides to end Night Cycle
void UShopDayCycle::InitializeDayCycle()
{
	timeElapsed = 0.f;
	GameState = EGameState::DAY;

	TimerDelegate.BindUFunction(this, "DayCycle");
	GetWorld()->GetTimerManager().SetTimer(DayTimer, TimerDelegate, timerRate, true);
}

// Function ticking on timerRate.
// Clears timer and initiate Night Cycle when dayCycleTime Ends
void UShopDayCycle::DayCycle()
{
	timeElapsed++;
	UE_LOG(LogTemp, Display, TEXT("Seconds elapsed: %f"), timeElapsed);

	if (timeElapsed >= dayCycleTime)
	{
		GetTimerManager().ClearTimer(DayTimer);
		GameState = EGameState::NIGHT;
	}
}
