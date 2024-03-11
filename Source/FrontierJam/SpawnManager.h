// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
// #include "FrontierJamCharacter.h"
#include "WashingMachine.h"
#include "ShopDayCycle.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "SpawnManager.generated.h"

UCLASS()
class FRONTIERJAM_API ASpawnManager : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SpawnMesh", meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* SpawnMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SpawnCollider", meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* SpawnCollider;

	UPROPERTY(VisibleAnywhere, Category = "MachineSpawnLocation", meta = (AlllowPrivateAccess = "true"))
		class UArrowComponent* MachineSpawnLocation;
	
public:	
	// Sets default values for this actor's properties
	ASpawnManager();

	UFUNCTION()
	void SpawnNewMachine();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
		void PlayerInteractionOnOverlap(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	// AFrontierJamCharacter* PlayerRef;
	UShopDayCycle* GameInstanceRef;

};
