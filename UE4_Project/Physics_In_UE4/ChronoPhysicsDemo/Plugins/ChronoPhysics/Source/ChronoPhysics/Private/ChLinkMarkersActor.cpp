// Fill out your copyright notice in the Description page of Project Settings.


#include "ChLinkMarkersActor.h"
#include "chrono/physics/ChLinkMarkers.h"
#include "ChBodyComponent.h"
#include "util.h"

void AChLinkMarkersActor::PhysicsObjectConstruct()
{
	this->ChData = std::make_shared<chrono::ChLinkMarkers>();
}


void AChLinkMarkersActor::ChLinkInitialize()
{
	Super::ChLinkInitialize();

	if (std::dynamic_pointer_cast<chrono::ChLinkMarkers>(this->ChData)) {
		auto body1 = Cast<UChBodyComponent>(this->target1->FindComponentByClass(UChBodyComponent::StaticClass()))->GetChData();
		auto body2 = Cast<UChBodyComponent>(this->target2->FindComponentByClass(UChBodyComponent::StaticClass()))->GetChData();
		auto pos = FVECTOR_TO_CHRONO_VEC(this->GetActorLocation());
		auto rot = FQUAT_TO_CHRONO_QUAT(FQuat(this->GetActorRotation()));
		if (body1 && body2 && std::dynamic_pointer_cast<chrono::ChLinkMarkers>(this->ChData)) {
			std::dynamic_pointer_cast<chrono::ChLinkMarkers>(this->ChData)->Initialize(body1, body2, chrono::ChCoordsys<>(pos, rot));
			this->isInitialized = true;
		}
	}
}
