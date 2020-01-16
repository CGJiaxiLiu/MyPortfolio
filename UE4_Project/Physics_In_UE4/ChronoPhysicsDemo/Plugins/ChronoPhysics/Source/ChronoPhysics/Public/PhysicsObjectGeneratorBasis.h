// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ChPhysicsObjectInterface.h"
#include "ChBody_GeneratedActor.h"
#include "ChBodyComponent.h"
#include "ChLinkLockActor.h"
#include "ChLink_EngineActor.h"
#include "PhysicsObjectGeneratorBasis.generated.h"

UENUM()
namespace EShapeType {
	enum Type {
		Box,
		Cylinder,
		Sphere
	};
}

UENUM()
namespace ELinkType {
	enum Type {
		LOCK,
		SPHERICAL,
		POINTPLANE,
		POINTLINE,
		CYLINDRICAL,
		PRISMATIC,
		PLANEPLANE,
		OLDHAM,
		REVOLUTE,
		FREE,
		ALIGN,
		PARALLEL,
		PERPEND,
		TRAJECTORY,
		CLEARANCE,
		REVOLUTEPRISMATIC,
		ENGINE_TORQUE,
		ENGINE_ROTATION,
		ENGINE_SPEED
	};
}

UCLASS()
class CHRONOPHYSICS_API APhysicsObjectGeneratorBasis : public AActor, public IChPhysicsObjectInterface
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* VisualAsset;

	UPROPERTY(EditAnywhere, Category = "Chrono|PhysicsObjectGenerator")
	TArray<TScriptInterface<IChPhysicsObjectInterface>> PhysicsObjectList;
	
	// Sets default values for this actor's properties
	APhysicsObjectGeneratorBasis();

	virtual void PhysicsObjectConstruct() override {}
	virtual void PhysicsObjectInitalize() override;
	virtual void AddToSystem(std::shared_ptr<chrono::ChSystem> phySystem) override;
	virtual void UpdatePhysicsState() override;
	virtual void UpdateVisualAsset() override;
	virtual void AddToSystem(TArray<TScriptInterface<IChPhysicsObjectInterface>>& objList) override {}
	
	UFUNCTION(BlueprintCallable, Category = "Chrono")
	bool SetEngineMotion(int index, float motion);
	UFUNCTION(BlueprintPure, Category = "Chrono")
	float GetEngineMotion(int index);

protected:
	class AChBody_GeneratedActor* NewChBodyActor(EShapeType::Type shape);
	class AChLinkActor* NewChLinkActor(ELinkType::Type linkType);
	TArray<class AChLink_EngineActor*> EngineList;

	UStaticMesh* boxMesh;
	UStaticMesh* sphereMesh;
	UStaticMesh* cylinderMesh;

};
