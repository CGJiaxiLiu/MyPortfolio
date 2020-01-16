// Fill out your copyright notice in the Description page of Project Settings.


#include "ChLink_UniversalSpringActor.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "chrono/physics/ChSystem.h"
#include "chrono/physics/ChLinkUniversal.h"
#include "ChBodyComponent.h"
#include "util.h"

// Sets default values
AChLink_UniversalSpringActor::AChLink_UniversalSpringActor()
{
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ArrowMesh(TEXT("/ChronoPhysics/BasicShapeMesh/Arrow.Arrow"));

	Arrow = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Arrow"));
	Arrow->SetStaticMesh(ArrowMesh.Object);
	Arrow->CastShadow = false;
	Arrow->bHiddenInGame = true;
	Arrow->Mobility = EComponentMobility::Movable;
	RootComponent = Arrow;
}

void AChLink_UniversalSpringActor::PhysicsObjectConstruct()
{
	this->ChLinkUniversal = std::make_shared<chrono::ChLinkUniversal>();
	this->ChLinkRot1 = std::make_shared<chrono::ChLinkRotSpringCB>();
	this->ChLinkRot2 = std::make_shared<chrono::ChLinkRotSpringCB>();
	this->myTorqueFuntor = MakeShared<MyUniversalTorque>(DampCoef, SpringCoef);
}

void AChLink_UniversalSpringActor::PhysicsObjectInitalize()
{
	if (target1 && target2 && ChLinkUniversal && ChLinkRot1 && ChLinkRot2) {
		auto body1 = Cast<UChBodyComponent>(this->target1->FindComponentByClass(UChBodyComponent::StaticClass()))->GetChData();
		auto body2 = Cast<UChBodyComponent>(this->target2->FindComponentByClass(UChBodyComponent::StaticClass()))->GetChData();
		Cast<UChBodyComponent>(this->target1->FindComponentByClass(UChBodyComponent::StaticClass()))->PhysicsObjectInitalize();
		Cast<UChBodyComponent>(this->target2->FindComponentByClass(UChBodyComponent::StaticClass()))->PhysicsObjectInitalize();

		if (body1 && body2) {
			auto pos = FVECTOR_TO_CHRONO_VEC(this->GetActorLocation());
			auto rot = FQUAT_TO_CHRONO_QUAT(FQuat(this->GetActorRotation()));

			std::dynamic_pointer_cast<chrono::ChLinkUniversal>(this->ChLinkUniversal)->Initialize(body1, body2, chrono::ChFrame<>(pos, rot));

			auto rot1 = rot * chrono::Q_from_AngX(chrono::CH_C_PI_2);
			auto rot2 = rot * chrono::Q_from_AngZ(chrono::CH_C_PI_2);

			std::dynamic_pointer_cast<chrono::ChLinkRotSpringCB>(this->ChLinkRot1)->Initialize(body1, body2, chrono::ChCoordsys<>(pos, rot1));
			std::dynamic_pointer_cast<chrono::ChLinkRotSpringCB>(this->ChLinkRot2)->Initialize(body1, body2, chrono::ChCoordsys<>(pos, rot2));
			std::dynamic_pointer_cast<chrono::ChLinkRotSpringCB>(ChLinkRot1)->RegisterTorqueFunctor(myTorqueFuntor.Get());
			std::dynamic_pointer_cast<chrono::ChLinkRotSpringCB>(ChLinkRot2)->RegisterTorqueFunctor(myTorqueFuntor.Get());

			this->isInitialized = true;
		}
	}
}

void AChLink_UniversalSpringActor::AddToSystem(std::shared_ptr<chrono::ChSystem> phySystem)
{
	if (this->isInitialized) {
		phySystem->AddLink(ChLinkUniversal);
		phySystem->AddLink(ChLinkRot1);
		phySystem->AddLink(ChLinkRot2);
	}
}
