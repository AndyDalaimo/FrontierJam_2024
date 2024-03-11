// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "SpawnManager.generated.h"

UCLASS()
class FRONTIERJAM_API ASpawnManager : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SpawnCollider", meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* SpawnCollider;
	
public:	
	// Sets default values for this actor's properties
	ASpawnManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
