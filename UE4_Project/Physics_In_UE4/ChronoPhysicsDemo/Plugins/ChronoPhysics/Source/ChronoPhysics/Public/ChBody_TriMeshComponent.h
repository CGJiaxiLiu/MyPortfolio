// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChBodyComponent.h"
#include "ChBody_TriMeshComponent.generated.h"

namespace chrono {
	namespace geometry {
		class ChTriangleMeshConnected;
	}
}

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CHRONOPHYSICS_API UChBody_TriMeshComponent : public UChBodyComponent
{
	GENERATED_BODY()
	
public:
	virtual void PhysicsObjectConstruct() override;

protected:
	std::shared_ptr<chrono::geometry::ChTriangleMeshConnected> triMesh;
};
