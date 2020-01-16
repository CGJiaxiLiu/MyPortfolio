// Fill out your copyright notice in the Description page of Project Settings.


#include "ChBody_BoxComponent.h"
#include "chrono/physics/ChBodyEasy.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Runtime/Engine/Classes/PhysicsEngine/BodySetup.h"
#include "chrono_parallel/collision/ChCollisionModelParallel.h"
#include "util.h"

void UChBody_BoxComponent::PhysicsObjectConstruct()
{
	Super::PhysicsObjectConstruct();
	
	auto rootComp = Cast<UStaticMeshComponent>(this->GetOwner()->GetRootComponent());
	auto scale = rootComp->RelativeScale3D;
	if (rootComp) {
		auto boxList = rootComp->GetStaticMesh()->BodySetup->AggGeom.BoxElems;
		if (boxList.Num()) {
			auto box = boxList[0];
			this->ChData = std::make_shared<chrono::ChBodyEasyBox>(box.X * scale.X / CHRONO_SCALE, box.Z * scale.Z / CHRONO_SCALE, box.Y * scale.Y / CHRONO_SCALE, Density, isCollide, false);
			if (isForParallel) {
				this->ChData->SetCollisionModel(std::make_shared<chrono::collision::ChCollisionModelParallel>());
				this->ChData->GetCollisionModel()->ClearModel();
				this->ChData->GetCollisionModel()->AddBox(box.X * scale.X / 2.0f / CHRONO_SCALE, box.Z * scale.Z / 2.0f / CHRONO_SCALE, box.Y * scale.Y / 2.0f / CHRONO_SCALE);
				this->ChData->GetCollisionModel()->BuildModel();
				this->ChData->SetCollide(true);
			}
			return;
		}
	}
}
