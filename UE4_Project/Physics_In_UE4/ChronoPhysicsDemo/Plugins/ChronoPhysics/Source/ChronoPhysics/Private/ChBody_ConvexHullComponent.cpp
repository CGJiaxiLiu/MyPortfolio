// Fill out your copyright notice in the Description page of Project Settings.


#include "ChBody_ConvexHullComponent.h"
#include "chrono/physics/ChBody.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Runtime/Engine/Classes/PhysicsEngine/BodySetup.h"
#include "chrono_parallel/collision/ChCollisionModelParallel.h"
#include "util.h"

void UChBody_ConvexHullComponent::PhysicsObjectConstruct()
{
	Super::PhysicsObjectConstruct();

	auto rootComp = Cast<UStaticMeshComponent>(this->GetOwner()->GetRootComponent());
	auto scale = rootComp->RelativeScale3D;
	if (rootComp) {
		auto convexList = rootComp->GetStaticMesh()->BodySetup->AggGeom.ConvexElems;
		this->ChData = std::make_shared<chrono::ChBody>();

		if (isForParallel) {
			ChData->SetCollisionModel(std::make_shared<chrono::collision::ChCollisionModelParallel>());
		}

		this->ChData->GetCollisionModel()->ClearModel();
		if (convexList.Num()) {
			for (auto convex : convexList) {
				std::vector<chrono::ChVector<>> pointlist;
				for (auto vtx : convex.VertexData) {
					pointlist.push_back(chrono::ChVector<>(vtx.X * scale.X / CHRONO_SCALE, vtx.Z * scale.Z / CHRONO_SCALE, vtx.Y * scale.Y / CHRONO_SCALE));
				}
				this->ChData->GetCollisionModel()->AddConvexHull(pointlist);
			}
			this->ChData->GetCollisionModel()->BuildModel();

			auto volume = rootComp->GetStaticMesh()->BodySetup->AggGeom.GetVolume(scale);
			double mass = volume / CHRONO_SCALE / CHRONO_SCALE / CHRONO_SCALE * Density;
			this->ChData->SetMass(mass);
			auto box = rootComp->GetStaticMesh()->BodySetup->AggGeom.CalcAABB(FTransform());
			FVector size = box.GetExtent() / CHRONO_SCALE;
			FVector internia = 1.0 / 12.0 * mass * FVector(pow(size.Y, 2) + pow(size.Z, 2), pow(size.X, 2) + pow(size.Z, 2), pow(size.X, 2) + pow(size.Y, 2));
			this->ChData->SetInertiaXX(chrono::ChVector<>(internia.X, internia.Z, internia.Y));
			return;
		}
	}
}

