// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "ChBody_GeneratedActor.generated.h"

/**
 * 
 */
UCLASS()
class CHRONOPHYSICS_API AChBody_GeneratedActor : public AStaticMeshActor
{
	GENERATED_BODY()

public:
	AChBody_GeneratedActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chroxy|ChBody")
	class UChBodyComponent* ChComp;
	
};