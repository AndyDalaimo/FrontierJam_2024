// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "ShopDayCycle.h"
#include "LaundryBag.h"
#include "ShopManager.h"
#include "Kismet/GameplayStatics.h"
#include "WashingMachine.generated.h"

UCLASS()
class FRONTIERJAM_API AWashingMachine : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MeshComponent", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* MachineMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* MachineCollisionComp;

	
public:	
	// Sets default values for this actor's properties
	AWashingMachine();

	UFUNCTION()
		void UpgradeMachine();

	UPROPERTY(EditAnywhere, Category = "Machine|Upgrade")
		int UpgradeCost;
	
	UPROPERTY(EditAnywhere, Category = "Machine|Upgrade")
		int UpgradeIncrease;

	UPROPERTY(EditAnywhere, Category = "Machine|WasCycle")
		uint8 ReputationIncrease;

	UPROPERTY(EditAnywhere, Category = "Machine|WashCycle")
		int WashCycleTime;

	UPROPERTY(EditAnywhere, Category = "Machine|WashCycle")
		float timerRate = 1.f;

	UPROPERTY(EditAnywhere, Category = "Machine|WashCycle")
		float WashReward = 10.f;

	UPROPERTY(EditAnywhere, Category = "Machine|WashCycle")
		float WashReward_Increase = 10.f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Machine|Upgrade")
	EMachineUpgrade UpgradeState;

	UFUNCTION()
		void MachineWashOnOverlap(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult);

	UFUNCTION()
		void WashCycle();

	// Anim Cue functions for BP
	UFUNCTION(BlueprintImplementableEvent)
		void K2_StartWashCycle();
	void K2_StartWashCycle_Implementable() { };
	
	UFUNCTION(BlueprintImplementableEvent)
		void K2_EndWashCycle();
	void K2_EndWashCycle_Implementable() { };

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	// Properties for in Launch timer
	FTimerHandle WashTimer;
	FTimerDelegate TimerDelegate;
	float washTimeElapsed;
	bool bWashing;

	// References to World Objects
	UShopDayCycle* GameInstanceRef;
	AShopManager* ShopManagerRef;

	// Mesh Instances for the Machine states
	UStaticMesh* MachineMeshRef;
};
