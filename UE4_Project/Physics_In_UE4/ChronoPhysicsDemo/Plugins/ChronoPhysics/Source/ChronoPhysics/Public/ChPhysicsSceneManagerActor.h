// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ChPhysicsObjectInterface.h"
#include <memory>
#include "ChPhysicsSceneManagerActor.generated.h"

namespace chrono {
	class ChSystem;
	class ChContactable;
}

UCLASS()
class CHRONOPHYSICS_API AChPhysicsSceneManagerActor : public AActor
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(EditAnywhere, Category = "Chrono|SolverParameter")
	int Substep = 5;

	UPROPERTY(EditAnywhere, Category = "Chrono|SolverParameter")
	float MaxStepLengthms = 6;

	UPROPERTY(EditAnywhere, Category = "Chrono|SolverParameter")
	int MaxItersSolverSpeed = 30;

	UPROPERTY(EditAnywhere, Category = "Chrono|SolverParameter")
	int MaxItersSolverStab = 10;

	UPROPERTY(EditAnywhere, Category = "Chrono|SolverParameter", meta = (EditConditionToggle))
	bool bSetDefaultCollisionParameter = true;

	UPROPERTY(EditAnywhere, Category = "Chrono|SolverParameter", meta = (editcondition = "bSetDefaultCollisionParameter"))
	float DefaultSuggestedEnvelope = 0.005;

	UPROPERTY(EditAnywhere, Category = "Chrono|SolverParameter", meta = (editcondition = "bSetDefaultCollisionParameter"))
	float DefaultSuggestedMargin = 0.0001;

	UPROPERTY(EditAnywhere, Category = "Chrono|SolverParameter", meta = (editcondition = "bSetDefaultCollisionParameter"))
	float SetContactBreakingThreshold = 0.0001;
	

	// Sets default values for this actor's properties
	AChPhysicsSceneManagerActor();
	virtual void Tick(float DeltaTime) override;

	virtual void SystemInitialize();
	virtual void FetchPhysicsObject();
	virtual void InitPhysicsObject();
	virtual void AddObjectToSystem();
	virtual void StepPhysics(float deltaTime);

	UFUNCTION(BlueprintCallable, Category = "Chrono")
	const TMap<FName, FExportData> ExportData();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	std::shared_ptr<chrono::ChSystem> phySystem;

	UPROPERTY(VisibleInstanceOnly)
	TArray<TScriptInterface<IChPhysicsObjectInterface>> PhysicsObjectList;
	
};

