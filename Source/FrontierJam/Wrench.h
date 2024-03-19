// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Wrench.generated.h"

UCLASS()
class FRONTIERJAM_API AWrench : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWrench();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* WrenchMesh;

	UFUNCTION()
		void DropWrench();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UStaticMesh* WrenchMeshAsset;

};
