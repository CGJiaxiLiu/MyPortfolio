// Fill out your copyright notice in the Description page of Project Settings.


#include "ChBodyComponent.h"
#include "GameFramework/Actor.h"
#include "chrono/physics/ChBody.h"
#include "chrono/physics/ChSystem.h"
#include "chrono_parallel/collision/ChCollisionModelParallel.h"
#include "util.h"


UChBodyComponent::UChBodyComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	
}

void UChBodyComponent::PhysicsObjectConstruct()
{
	//DataOnwnerName = FName(*(GetOwner()->GetName() + GetName()));

	this->ChData = std::make_shared<chrono::ChBody>();
	this->GetOwner()->GetRootComponent()->SetMobility(EComponentMobility::Movable);

	//if (!isFixed) {
	//	this->GetOwner()->GetRootComponent()->SetMobility(EComponentMobility::Movable);
	//}
	//else {
	//	this->GetOwner()->GetRootComponent()->SetMobility(EComponentMobility::Static);
	//}

	if (isForParallel) {
		ChData->SetCollisionModel(std::make_shared<chrono::collision::ChCollisionModelParallel>());
	}
}

void UChBodyComponent::PhysicsObjectInitalize()
{
	if (this->ChData && !this->isInitialized) {
		this->ChData->SetBodyFixed(this->isFixed);
		this->ChData->SetCollide(this->isCollide);
		this->ChData->SetLimitSpeed(true);
		this->ChData->SetMaxSpeed(this->MaxSpeed);
		this->ChData->SetMaxWvel(this->MaxAngularSpeed);

		auto pos = FVECTOR_TO_CHRONO_VEC(GetOwner()->GetRootComponent()->GetComponentLocation());
		this->ChData->SetPos(pos);
		auto quat = FQUAT_TO_CHRONO_QUAT(GetOwner()->GetRootComponent()->GetComponentQuat());
		this->ChData->SetRot(quat);

		if (this->CollisionFamily >= 0 && this->NoCollisionWithFamiliy.Num() > 0) {
			this->ChData->GetCollisionModel()->SetFamily(this->CollisionFamily);
			for (int noCollide : this->NoCollisionWithFamiliy) {
				this->ChData->GetCollisionModel()->SetFamilyMaskNoCollisionWithFamily(noCollide);
			}
		}

		this->ChData->GetMaterialSurfaceNSC()->SetSfriction(this->StaticFriction);
		this->ChData->GetMaterialSurfaceNSC()->SetKfriction(this->SlidingFriction);
		this->ChData->GetMaterialSurfaceNSC()->SetRollingFriction(this->RollingFriction);
		this->ChData->GetMaterialSurfaceNSC()->SetSpinningFriction(this->SpinningFriction);
		this->ChData->GetMaterialSurfaceNSC()->SetCohesion(this->Cohension);
		this->ChData->GetMaterialSurfaceNSC()->SetRestitution(this->Restitution);
		this->ChData->GetMaterialSurfaceNSC()->SetDampingF(this->DampingF);
		this->ChData->GetMaterialSurfaceNSC()->SetCompliance(this->Compliance);
		this->ChData->GetMaterialSurfaceNSC()->SetComplianceT(this->ComplianceT);
		this->ChData->GetMaterialSurfaceNSC()->SetComplianceRolling(this->ComplianceRoll);
		this->ChData->GetMaterialSurfaceNSC()->SetComplianceSpinning(this->ComplianceSpin);

		this->isInitialized = true;
	}
}

void UChBodyComponent::AddToSystem(std::shared_ptr<chrono::ChSystem> phySystem)
{
	if (this->isInitialized) {
		phySystem->AddBody(this->ChData);
	}
}

void UChBodyComponent::UpdatePhysicsState()
{
	if (this->isInitialized && this->bUseDrag) {
		this->ChData->Empty_forces_accumulators();
		ChData->Accumulate_force(this->ChData->GetPos_dt() * -DragCoefTrans, ChData->GetPos(), false);
		ChData->Accumulate_torque(ChData->GetWvel_par() * -DragCoefRot, false);

		if (this->bAddCustomForce) {
			ChData->Accumulate_force(FVECTOR_TO_CHRONO_VEC(CustomForce) * 100, 0, true);
		}
		//ChData->SetPos_dt(ChData->GetPos_dt()*0.98f);
		//ChData->SetRot_dt(ChData->GetRot_dt()*0.98f);
	}
	else if (this->isInitialized && this->bAddCustomForce) {
		this->ChData->Empty_forces_accumulators();
		ChData->Accumulate_force(FVECTOR_TO_CHRONO_VEC(CustomForce) * 100, 0, true);
	}

}

void UChBodyComponent::UpdateVisualAsset()
{
	if (isInitialized && !isFixed) {
		auto pos = CHRONO_VEC_TO_FVECTOR(this->ChData->GetPos());
		this->GetOwner()->SetActorLocation(pos);
		auto quat = CHRONO_QUAT_TO_FQUAT(this->ChData->GetRot());
		this->GetOwner()->SetActorRotation(quat);
	}
}

FExportData UChBodyComponent::ExportData()
{
	FExportData data;
	data.onwerName = this->BodyExportDataOwnerName;

	if (bExportData) {
		if (bExportPos) {
			data.data.Add(FName("Pos"), CHRONO_VEC_TO_FVECTOR((ChData->GetPos())));
		}
		if (bExportVel) {
			data.data.Add(FName("Vel"), CHRONO_VEC_TO_FVECTOR((ChData->GetPos_dt())));
		}
		if (bExportAcce) {
			data.data.Add(FName("Acce"), CHRONO_VEC_TO_FVECTOR((ChData->GetPos_dtdt())));
		}
		if (bExportAngVel) {
			data.data.Add(FName("AngVel"), CHRONO_VEC_TO_FVECTOR((ChData->GetWvel_loc())));
		}
		if (bExportContactForce) {
			data.data.Add(FName("ContactForce"), CHRONO_VEC_TO_FVECTOR((ChData->GetContactForce())));
		}
	}

	return data;
}
