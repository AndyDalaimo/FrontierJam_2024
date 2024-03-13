// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Info.h"
#include "LaundryBag.h"
#include "ShopDayCycle.h"
#include "ShopManager.h"
#include "Kismet/GameplayStatics.h"
#include "LaundryManager.generated.h"

/**
 * 
 */
UCLASS()
class FRONTIERJAM_API ALaundryManager : public AInfo
{
	GENERATED_BODY()

public:
	ALaundryManager();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LaundrySpawning")
		TArray<ALaundryBag*> BagReferences;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LaundrySpawning")
		float timerRate;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LaundrySpawning")
		TArray<FVector> PossibleSpawnLocations;
	
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LaundrySpawning")
	float timerDecreaseIncrement;

protected:
	virtual void BeginPlay();

	UFUNCTION(BlueprintCallable)
		void InitializeLaundrySpawnTimer();

	UFUNCTION()
		void LaundrySpawnTimerHandler();

	UFUNCTION(BlueprintCallable)
		FVector GetLaundrySpawnLocation();

	UFUNCTION(BLueprintCallable)
		void ReputationUpdate();

	UFUNCTION(BlueprintCallable)
		void DestroyAllLeftoverBags();

private:

	FActorSpawnParameters params;

	UShopDayCycle* GameInstanceRef;
	AShopManager* ShopManagerRef;

	// Properties for in game timer
	FTimerHandle SpawnTimer;
	FTimerDelegate TimerDelegate;
	
};
