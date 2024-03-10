// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Info.h"
#include "ShopManager.generated.h"

// Machine Upgrade states
UENUM(BlueprintType)
enum class EMachineUpgrade : uint8
{
	SMALL = 0 UMETA(DisplayName = "Small"),
	MEDIUM = 1 UMETA(DisplayName = "Medium"),
	LARGE = 2 UMETA(DisplayName = "Large"),
};


// Enum State tracker to influence UI Reviews and Store efficiency/Cleanliness
UENUM(BlueprintType)
enum class EReputationState : uint8
{
	NONE = 0 UMETA(Hidden),
	NEGATIVE = 1 UMETA(DisplayName = "Negative"),
	NEUTRAL = 2 UMETA(DisplayName = "Neutral"),
	POSITIVE = 3 UMETA(DisplayName = "Positive"),
	EXCELLENT = 4 UMETA(DisplayName = "Excellent"),
};


// Economy will track cash flow in and out 
USTRUCT(BlueprintType)
struct FShopEconomy
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Economy")
	float Cash;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Economy|Utilities")
	float UtilitiesCost;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Economy|Utilities")
	float MaintenanceCost;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Economy|Rent")
	float Rent;

	// Default Values to initiate Store
	void init()
	{
		Cash = 100.f;
		UtilitiesCost = 10.f;
		MaintenanceCost = 0.f;
		Rent = 10.f;
	}

};

// Store Reputation influenced by Player's actions in the game
USTRUCT(BlueprintType)
struct FShopReputation
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Reputation")
	EReputationState RepState;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Reputation|Variables")
	uint8 Clealiness;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Reputation|Variables")
	uint8 Decor;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Reputation")
	int32 Reputation;

};

/**
 * 
 */
UCLASS()
class FRONTIERJAM_API AShopManager : public AInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Shop|Economy")
	FShopEconomy Economy;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Shop|Reputation")
	FShopReputation Reputation;
	
};
