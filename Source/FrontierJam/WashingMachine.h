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

UENUM(BlueprintType)
enum class EChaosState : uint8
{
	WORKING = 0 UMETA(DisplayName = "Working"),
	BROKEN = 1 UMETA(Displayname = "Broken"),
};

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

	UFUNCTION()
		void FixMachine();

	UPROPERTY(EditAnywhere, Category = "MachineProperties|Upgrade")
		int UpgradeCost;
	
	UPROPERTY(EditAnywhere, Category = "MachineProperties|Upgrade")
		int UpgradeIncrease;

	UPROPERTY(EditAnywhere, Category = "MachineProperties|WashCycle")
		uint8 ReputationIncrease;
	
	UPROPERTY(EditAnywhere, Category = "MachineProperties|WashCycle")
		float ChoasChancePercent;
	
	UPROPERTY(EditAnywhere, Category = "MachineProperties|WashCycle")
		float ChoasChancePercentDecrease;

	UPROPERTY(EditAnywhere, Category = "MachineProperties|WashCycle")
		int WashCycleTime;

	UPROPERTY(EditAnywhere, Category = "MachineProperties|WashCycle")
		float timerRate = 1.f;

	UPROPERTY(EditAnywhere, Category = "MachineProperties|WashCycle")
		float WashReward = 10.f;

	UPROPERTY(EditAnywhere, Category = "MachineProperties|WashCycle")
		float WashReward_Increase = 10.f;
	
	UPROPERTY(VisibleAnywhere, Category = "MachineProperties|WashCycle")
		bool bWashing;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MachineProperties|Upgrade")
	EMachineUpgrade UpgradeState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MachineProperties|Upgrade")
	EChaosState ChaosState;


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

	UFUNCTION(BlueprintImplementableEvent)
		void K2_InitChaosEvent();
	void K2_InitChaosEvent_Implementable() { };
	
	UFUNCTION(BlueprintImplementableEvent)
		void K2_EndChaosEvent();
	void K2_EndChaosEvent_Implementable() { };

	UFUNCTION(BlueprintImplementableEvent)
		void K2_UpgradeEvent();
	void K2_UpgradeEvent_Implementable() { };


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	// Properties for in Launch timer
	FTimerHandle WashTimer;
	FTimerDelegate TimerDelegate;
	float washTimeElapsed;

	// References to World Objects
	UShopDayCycle* GameInstanceRef;
	AShopManager* ShopManagerRef;

	// Mesh Instances for the Machine states
	UStaticMesh* MachineMeshRef;
};
