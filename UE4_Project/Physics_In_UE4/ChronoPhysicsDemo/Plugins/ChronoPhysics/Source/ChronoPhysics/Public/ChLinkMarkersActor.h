// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChLinkActor.h"
#include "ChLinkMarkersActor.generated.h"


UCLASS()
class CHRONOPHYSICS_API AChLinkMarkersActor : public AChLinkActor
{
	GENERATED_BODY()

public:
	virtual void PhysicsObjectConstruct() override;
	virtual void ChLinkInitialize() override;
	
};
