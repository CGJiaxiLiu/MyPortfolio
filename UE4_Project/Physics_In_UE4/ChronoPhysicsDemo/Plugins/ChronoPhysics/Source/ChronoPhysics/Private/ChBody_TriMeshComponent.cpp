// Fill out your copyright notice in the Description page of Project Settings.


#include "ChBody_TriMeshComponent.h"
#include "chrono/physics/ChBody.h"
#include "chrono/geometry/ChTriangleMeshConnected.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Runtime/Engine/Classes/PhysicsEngine/BodySetup.h"
#include "Math/Box.h"
#include "chrono_parallel/collision/ChCollisionModelParallel.h"
#include "Interfaces/Interface_CollisionDataProvider.h"
#include "util.h"

void UChBody_TriMeshComponent::PhysicsObjectConstruct()
{
	Super::PhysicsObjectConstruct();

	auto rootComp = Cast<UStaticMeshComponent>(this->GetOwner()->GetRootComponent());
	auto scale = rootComp->RelativeScale3D;
	if (rootComp) {
		auto staticMesh = rootComp->GetStaticMesh();
		FTriMeshCollisionData* triMeshData = new FTriMeshCollisionData();
		if (staticMesh->GetPhysicsTriMeshData(triMeshData, true)) {
			this->triMesh = std::make_shared<chrono::geometry::ChTriangleMeshConnected>();
			this->ChData = std::make_shared<chrono::ChBody>();
			for (auto index : triMeshData->Indices) {
				auto vtx0 = chrono::ChVector<>(triMeshData->Vertices[index.v0].X * scale.X / CHRONO_SCALE, triMeshData->Vertices[index.v0].Z * scale.Z / CHRONO_SCALE, triMeshData->Vertices[index.v0].Y * scale.Y / CHRONO_SCALE);
				auto vtx1 = chrono::ChVector<>(triMeshData->Vertices[index.v1].X * scale.X / CHRONO_SCALE, triMeshData->Vertices[index.v1].Z * scale.Z / CHRONO_SCALE, triMeshData->Vertices[index.v1].Y * scale.Y / CHRONO_SCALE);
				auto vtx2 = chrono::ChVector<>(triMeshData->Vertices[index.v2].X * scale.X / CHRONO_SCALE, triMeshData->Vertices[index.v2].Z * scale.Z / CHRONO_SCALE, triMeshData->Vertices[index.v2].Y * scale.Y / CHRONO_SCALE);
				this->triMesh->addTriangle(vtx0, vtx1, vtx2);
			}
			this->triMesh->RepairDuplicateVertexes();
			if (isForParallel) {
				ChData->SetCollisionModel(std::make_shared<chrono::collision::ChCollisionModelParallel>());
			}
			this->ChData->GetCollisionModel()->ClearModel();
			this->ChData->GetCollisionModel()->AddTriangleMesh(this->triMesh, isFixed, false);
			this->ChData->GetCollisionModel()->BuildModel();

			auto volume = rootComp->GetStaticMesh()->BodySetup->AggGeom.GetVolume(scale);
			double mass = volume / CHRONO_SCALE / CHRONO_SCALE / CHRONO_SCALE * Density;
			this->ChData->SetMass(mass);
			auto box = rootComp->GetStaticMesh()->BodySetup->AggGeom.CalcAABB(FTransform());
			FVector size = box.GetExtent() / CHRONO_SCALE;
			FVector internia = 1.0 / 12.0 * mass * FVector(pow(size.Y, 2) + pow(size.Z, 2), pow(size.X, 2) + pow(size.Z, 2), pow(size.X, 2) + pow(size.Y, 2));
			this->ChData->SetInertiaXX(chrono::ChVector<>(internia.X, internia.Z, internia.Y));
		}
	}
}

