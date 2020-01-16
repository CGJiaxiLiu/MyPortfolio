// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PhysicsObjectGeneratorBasis.h"
#include "PhysicsObjectGeneratorSample.generated.h"

/**
 * 
 */
UCLASS()
class CHRONOPHYSICS_API APhysicsObjectGeneratorSample : public APhysicsObjectGeneratorBasis
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Chrono|PhysicsObjectGeneratorSample")
	int BoxCount = 10;

	virtual void PhysicsObjectConstruct() override;
};
