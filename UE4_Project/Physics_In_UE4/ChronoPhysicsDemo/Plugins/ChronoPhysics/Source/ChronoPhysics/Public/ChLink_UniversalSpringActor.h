// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ChPhysicsObjectInterface.h"
#include <memory>
#include "chrono/physics/ChLinkRotSpringCB.h"
#include "ChLink_UniversalSpringActor.generated.h"

namespace chrono {
	class ChLink;
}

class MyUniversalTorque : public chrono::ChLinkRotSpringCB::TorqueFunctor
{
public:
	float dampCoef;
	float springCoef;
	float restAngle = 0;

	MyUniversalTorque() : dampCoef(0.1f), springCoef(50.f) {}
	MyUniversalTorque(float InDamp, float InK) : dampCoef(InDamp), springCoef(InK) {}
	inline void SetSpringCoef(float InK) { this->springCoef = InK; }
	inline void SetDampCoef(float InDamp) { this->dampCoef = InDamp; }
	inline void SetRestAngle(float angle) { this->restAngle = angle; }
	inline virtual double operator()(double time, double angle, double vel, chrono::ChLinkRotSpringCB* link) override { return -springCoef * (angle - restAngle) - dampCoef * vel; }
};

UCLASS()
class CHRONOPHYSICS_API AChLink_UniversalSpringActor : public AActor, public IChPhysicsObjectInterface
{
	GENERATED_BODY()
	
public:	

	UPROPERTY(VisibleInstanceOnly)
	class UStaticMeshComponent* Arrow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chrono")
	AActor* target1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chrono")
	AActor* target2;

	UPROPERTY(EditAnywhere, Category = "Chrono|SpringParameter")
	float DampCoef = 10;

	UPROPERTY(EditAnywhere, Category = "Chrono|SpringParameter")
	float SpringCoef = 100;

	AChLink_UniversalSpringActor();

	virtual void PhysicsObjectConstruct() override;
	virtual void PhysicsObjectInitalize() override;
	virtual void AddToSystem(std::shared_ptr<chrono::ChSystem> phySystem) override;
	virtual void UpdatePhysicsState() override {}
	virtual void UpdateVisualAsset() override {}

protected:
	std::shared_ptr<chrono::ChLink> ChLinkUniversal;
	std::shared_ptr<chrono::ChLink> ChLinkRot1;
	std::shared_ptr<chrono::ChLink> ChLinkRot2;
	TSharedPtr<chrono::ChLinkRotSpringCB::TorqueFunctor> myTorqueFuntor;

};
