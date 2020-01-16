// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PhysicsObjectGeneratorBasis.h"
#include "PhysicsObjectGeneratorJson.generated.h"

/**
 * Generate Phyiscs Object From Json File
 */
UCLASS()
class CHRONOPHYSICS_API APhysicsObjectGeneratorJson : public APhysicsObjectGeneratorBasis
{
	GENERATED_BODY()

public:
	//UPROPERTY(EditAnywhere, Category = "Chrono|PhysicsObjectGeneratorJson", meta = (EditConditionToggle))
	//bool bGenerateRelative = false;
	//UPROPERTY(EditAnywhere, Category = "Chrono|PhysicsObjectGeneratorJson", meta = (editcondition = "bGenerateRelative"))
	//bool bPositionRelative = false;
	//UPROPERTY(EditAnywhere, Category = "Chrono|PhysicsObjectGeneratorJson", meta = (editcondition = "bGenerateRelative"))
	//bool bRotationRelative = false;

	//Json File Location
	UPROPERTY(EditAnywhere, Category = "Chrono|PhysicsObjectGeneratorJson")
	FString JsonFileName;
	UPROPERTY(EditAnywhere, Category = "Chrono|PhysicsObjectGeneratorJson")
	bool bPositionRelative = false;

	virtual void PhysicsObjectConstruct() override;
};
