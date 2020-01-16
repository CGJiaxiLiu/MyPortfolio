// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChLinkMarkersActor.h"
#include "ChLink_SpringActor.generated.h"

/**
 * 
 */
UCLASS()
class CHRONOPHYSICS_API AChLink_SpringActor : public AChLinkMarkersActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "Chrono|SpringParameter")
	float SpringCoef = 1000;

	UPROPERTY(EditAnywhere, Category = "Chrono|SpringParameter")
	float DampCoef = 50;


	virtual void PhysicsObjectConstruct() override;
	virtual void ChLinkInitialize() override;
};
