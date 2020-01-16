// Fill out your copyright notice in the Description page of Project Settings.


#include "ChBody_SphereComponent.h"
#include "chrono/physics/ChBodyEasy.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Runtime/Engine/Classes/PhysicsEngine/BodySetup.h"
#include "chrono_parallel/collision/ChCollisionModelParallel.h"
#include "util.h"

void UChBody_SphereComponent::PhysicsObjectConstruct()
{
	Super::PhysicsObjectConstruct();

	auto rootComp = Cast<UStaticMeshComponent>(this->GetOwner()->GetRootComponent());
	auto scale = rootComp->RelativeScale3D;
	if (rootComp) {
		auto sphereList = rootComp->GetStaticMesh()->BodySetup->AggGeom.SphereElems;
		if (sphereList.Num()) {
			auto sphere = sphereList[0];
			this->ChData = std::make_shared<chrono::ChBodyEasySphere>(sphere.Radius / CHRONO_SCALE * scale.X, Density, isCollide, false);
			//this->ChData = std::make_shared<chrono::ChBodyEasyEllipsoid>(FVECTOR_TO_CHRONO_VEC(FVector(sphere.Radius * scale.X, sphere.Radius * scale.Y, sphere.Radius * scale.Z)), Density, isCollide, false);
			if (isForParallel) {
				this->ChData->SetCollisionModel(std::make_shared<chrono::collision::ChCollisionModelParallel>());
				this->ChData->GetCollisionModel()->ClearModel();
				this->ChData->GetCollisionModel()->AddSphere(sphere.Radius / CHRONO_SCALE * scale.X);
				this->ChData->GetCollisionModel()->BuildModel();
				this->ChData->SetCollide(true);
			}
		}
	}
}

