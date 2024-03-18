// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WashingMachine.h"
#include "ShopDayCycle.h"
#include "ShopManager.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "SpawnManager.generated.h"

UENUM(BlueprintType)
enum class EItemToSpawn : uint8
{
	MACHINE = 0 UMETA(DisplayName = "WashingMachine"),
	DECOR = 1 UMETA(Displayname = "Decor")
};

UCLASS()
class FRONTIERJAM_API ASpawnManager : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SpawnMesh", meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* SpawnMesh;

	UPROPERTY(VisibleAnywhere, Category = "MachineSpawnLocation", meta = (AlllowPrivateAccess = "true"))
		class UArrowComponent* MachineSpawnLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnParameters|DecorItemToSpawn", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<AActor> DecorBlueprintClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnParameters|MachineToSpawn", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<AActor> MachineBlueprintClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnParameters|DecorItemToSpawn", meta = (AllowPrivateAccess = "true"))
		int ReputationIncrease;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnParameters", meta = (AllowPrivateAccess = "true"))
		int ItemCost;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnParameters", meta = (AllowPrivateAccess = "true"))
		FRotator SpawnRotation;
	
public:	
	// Sets default values for this actor's properties
	ASpawnManager();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnParameters")
		EItemToSpawn ItemToSpawn;

	UFUNCTION()
	void SpawnNewMachine();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:


	UShopDayCycle* GameInstanceRef;
	AShopManager* ShopManagerRef;


};
