// Fill out your copyright notice in the Description page of Project Settings.


#include "ChLink_UniversalActor.h"
#include "chrono/physics/ChLinkUniversal.h"
#include "ChBodyComponent.h"
#include "util.h"

void AChLink_UniversalActor::PhysicsObjectConstruct()
{
	this->ChData = std::make_shared<chrono::ChLinkUniversal>();
}

void AChLink_UniversalActor::ChLinkInitialize()
{
	Super::ChLinkInitialize();

	if (std::dynamic_pointer_cast<chrono::ChLinkUniversal>(this->ChData)) {
		auto body1 = Cast<UChBodyComponent>(this->target1->FindComponentByClass(UChBodyComponent::StaticClass()))->GetChData();
		auto body2 = Cast<UChBodyComponent>(this->target2->FindComponentByClass(UChBodyComponent::StaticClass()))->GetChData();
		auto pos = FVECTOR_TO_CHRONO_VEC(this->GetActorLocation());
		auto rot = FQUAT_TO_CHRONO_QUAT(FQuat(this->GetActorRotation()))/* * chrono::Q_from_AngX(chrono::CH_C_PI_2)*/;
		std::dynamic_pointer_cast<chrono::ChLinkUniversal>(this->ChData)->Initialize(body1, body2, chrono::ChFrame<>(pos, rot));
		this->isInitialized = true;
	}

}
