// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "ShopDayCycle.h"
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

	UPROPERTY(EditDefaultsOnly, Category = "Machine|Upgrade")
		int UpgradeCost;

	UPROPERTY(EditDefaultsOnly, Category = "Machine|Upgrade")
		int UpgradeIncrease;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Machine|Upgrade")
	EMachineUpgrade UpgradeState;

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

	// References to World Objects
	UShopDayCycle* GameInstanceRef;
	AShopManager* ShopManagerRef;

	// Mesh Instances for the Machine states
	UStaticMesh* MachineMeshRef;
};
