// Fill out your copyright notice in the Description page of Project Settings.


#include "LaundryManager.h"

#include "TimerManager.h"

ALaundryManager::ALaundryManager() : timerRate(2.f)
{
	
}


void ALaundryManager::BeginPlay()
{
	Super::BeginPlay();

	GameInstanceRef = Cast<UShopDayCycle>(GetWorld()->GetGameInstance());

	TimerDelegate.BindUFunction(this, "LaundrySpawnTimerHandler");
	GetWorld()->GetTimerManager().SetTimer(SpawnTimer, TimerDelegate, timerRate, true);
}

void ALaundryManager::InitializeLaundrySpawnTimer()
{
	TimerDelegate.BindUFunction(this, "LaundrySpawnTimerHandler");
	GetWorld()->GetTimerManager().SetTimer(SpawnTimer, TimerDelegate, timerRate, true);
}


void ALaundryManager::LaundrySpawnTimerHandler()
{
	if (GameInstanceRef->GameState == EGameState::DAY)
	{
		BagReferences.Push(Cast<ALaundryBag>(GetWorld()->SpawnActor<ALaundryBag>(GetLaundrySpawnLocation(), FRotator(0,0,0), params)));
	}
	else {
		GameInstanceRef->GetTimerManager().ClearTimer(SpawnTimer);
	}
}

// Return one of the possible spawn locations for the laundry bags
FVector ALaundryManager::GetLaundrySpawnLocation()
{
	if (!PossibleSpawnLocations.IsEmpty())
	{
		int RandomLocationIndex = FMath::RandRange(0, PossibleSpawnLocations.Num() - 1);
		UE_LOG(LogTemp, Warning, TEXT("Random Location Index: %d"), RandomLocationIndex);
		return PossibleSpawnLocations[RandomLocationIndex];
	}
	else {
		return FVector(0, 0, 0);
	}

}
