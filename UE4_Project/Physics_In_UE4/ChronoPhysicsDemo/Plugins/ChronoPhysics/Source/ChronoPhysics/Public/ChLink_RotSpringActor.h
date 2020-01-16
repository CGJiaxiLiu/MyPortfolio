// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChLinkMarkersActor.h"
#include "chrono/physics/ChLinkRotSpringCB.h"
#include "ChLink_RotSpringActor.generated.h"


class MyTorque : public chrono::ChLinkRotSpringCB::TorqueFunctor
{
public:
	float dampCoef;
	float springCoef;
	float restAngle = 0;

	MyTorque() : dampCoef(0.1f), springCoef(50.f) {}
	MyTorque(float InDamp, float InK) : dampCoef(InDamp), springCoef(InK) {}
	inline void SetSpringCoef(float InK) { this->springCoef = InK; }
	inline void SetDampCoef(float InDamp) { this->dampCoef = InDamp; }
	inline void SetRestAngle(float angle) { this->restAngle = angle; }
	inline virtual double operator()(double time, double angle, double vel, chrono::ChLinkRotSpringCB* link) override { return -springCoef * (angle - restAngle) - dampCoef * vel; }
};

UCLASS()
class CHRONOPHYSICS_API AChLink_RotSpringActor : public AChLinkMarkersActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "Chrono|SpringParameter")
	float DampCoef = 10;
	UPROPERTY(EditAnywhere, Category = "Chrono|SpringParameter")
	float SpringCoef = 100;

	virtual void PhysicsObjectConstruct() override;
	virtual void ChLinkInitialize() override;

protected:
	TSharedPtr<chrono::ChLinkRotSpringCB::TorqueFunctor> myTorqueFuntor;
};
