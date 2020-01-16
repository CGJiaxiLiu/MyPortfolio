// Fill out your copyright notice in the Description page of Project Settings.


#include "ChBody_CylinderComponent.h"
#include "chrono/physics/ChBodyEasy.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Runtime/Engine/Classes/PhysicsEngine/BodySetup.h"
#include "chrono_parallel/collision/ChCollisionModelParallel.h"
#include "util.h"

void UChBody_CylinderComponent::PhysicsObjectConstruct()
{
	Super::PhysicsObjectConstruct();

	auto rootComp = Cast<UStaticMeshComponent>(this->GetOwner()->GetRootComponent());
	auto scale = rootComp->RelativeScale3D;
	if (rootComp) {
		auto cylinderList = rootComp->GetStaticMesh()->BodySetup->AggGeom.SphylElems;
		if (cylinderList.Num()) {
			auto cylinder = cylinderList[0];
			this->ChData = std::make_shared<chrono::ChBodyEasyCylinder>(cylinder.Radius * scale.X / CHRONO_SCALE, cylinder.Length * scale.Z / CHRONO_SCALE, Density, isCollide, false);
			if (isForParallel) {
				this->ChData->SetCollisionModel(std::make_shared<chrono::collision::ChCollisionModelParallel>());
				this->ChData->GetCollisionModel()->ClearModel();
				this->ChData->GetCollisionModel()->AddCylinder(cylinder.Radius * scale.X / CHRONO_SCALE, cylinder.Radius * scale.X / CHRONO_SCALE, cylinder.Length * scale.Z / CHRONO_SCALE * 0.5);
				this->ChData->GetCollisionModel()->BuildModel();
				this->ChData->SetCollide(true);
			}
			return;
		}
	}
}
