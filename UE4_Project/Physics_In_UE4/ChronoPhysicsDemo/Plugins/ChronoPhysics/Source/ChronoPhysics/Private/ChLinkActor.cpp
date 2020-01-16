// Fill out your copyright notice in the Description page of Project Settings.


#include "ChLinkActor.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "ChBodyComponent.h"
#include "chrono/physics/ChLink.h"
#include "chrono/physics/ChSystem.h"
#include "util.h"

AChLinkActor::AChLinkActor()
{
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ArrowMesh(TEXT("/ChronoPhysics/BasicShapeMesh/Arrow.Arrow"));

	Arrow = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Arrow"));
	Arrow->SetStaticMesh(ArrowMesh.Object);
	Arrow->CastShadow = false;
	Arrow->bHiddenInGame = true;
	RootComponent = Arrow;

}

void AChLinkActor::PhysicsObjectConstruct()
{
	this->ChData = std::make_shared<chrono::ChLink>();
}

void AChLinkActor::PhysicsObjectInitalize()
{
	if (!this->IsReadyForInitialize()) {
		return;
	}
	this->ChLinkInitialize();
}

void AChLinkActor::AddToSystem(std::shared_ptr<chrono::ChSystem> phySystem)
{
	if (this->isInitialized) {
		phySystem->AddLink(this->ChData);
	}
}

FExportData AChLinkActor::ExportData()
{
	FExportData data;
	data.onwerName = this->LinkExportDataOwnerName;

	if (bExportData) {
		if (bExportReactForce) {
			data.data.Add(FName("ReactForce"), CHRONO_VEC_TO_FVECTOR((ChData->Get_react_force())) / 100.0f);
		}
		if (bExportReactTorque) {
			data.data.Add(FName("ReactTorque"), CHRONO_VEC_TO_FVECTOR((ChData->Get_react_torque())) / 100.0f);
		}
	}

	return data;
}

bool AChLinkActor::IsReadyForInitialize()
{
	if (target1 && target2 && ChData) {
		auto comp1 = Cast<UChBodyComponent>(this->target1->FindComponentByClass(UChBodyComponent::StaticClass()));
		auto comp2 = Cast<UChBodyComponent>(this->target2->FindComponentByClass(UChBodyComponent::StaticClass()));
		if (comp1 && comp2) {
			comp1->PhysicsObjectInitalize();
			comp2->PhysicsObjectInitalize();
			return true;
		}
	}
	return false;
}
