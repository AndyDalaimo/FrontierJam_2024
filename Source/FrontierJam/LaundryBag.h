// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/IPhysicsComponent.h"
#include "Kismet/GameplayStatics.h"
#include "LaundryBag.generated.h"

UCLASS()
class FRONTIERJAM_API ALaundryBag : public AActor
{
	GENERATED_BODY()


	
public:	
	// Sets default values for this actor's properties
	ALaundryBag();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* LaundryMesh;

	UFUNCTION()
		void Launch(FVector LaunchDir);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UStaticMesh* LaundryMeshAsset;

	// Properties for in Launch timer
	FTimerHandle LaunchTimer;
	FTimerDelegate TimerDelegate;

};
