// Fill out your copyright notice in the Description page of Project Settings.


#include "ChLink_EngineActor.h"
#include "chrono/physics/ChLinkEngine.h"
#include "UObject/ConstructorHelpers.h"
#include "Curves/CurveFloat.h"
#include "util.h"

AChLink_EngineActor::AChLink_EngineActor()
{
	static ConstructorHelpers::FObjectFinder<UCurveFloat> curveFinder(TEXT("/ChronoPhysics/EngineCurve/EngineCurve0.EngineCurve0"));
	Curve = curveFinder.Object;
}

void AChLink_EngineActor::PhysicsObjectConstruct()
{
	auto linkptr = std::make_shared<chrono::ChLinkEngine>();
	switch (EngineType) {

	case EEngineMode::ENG_MODE_ROTATION:
		linkptr->Set_eng_mode(chrono::ChLinkEngine::eCh_eng_mode::ENG_MODE_ROTATION);
		break;

	case EEngineMode::ENG_MODE_SPEED:
		linkptr->Set_eng_mode(chrono::ChLinkEngine::eCh_eng_mode::ENG_MODE_SPEED);
		break;

	case EEngineMode::ENG_MODE_TORQUE:
		linkptr->Set_eng_mode(chrono::ChLinkEngine::eCh_eng_mode::ENG_MODE_TORQUE);
		break;
	}

	this->ChData = linkptr;
}

void AChLink_EngineActor::UpdatePhysicsState()
{
	if (EngineType == EEngineMode::ENG_MODE_ROTATION) {
		auto engine = std::dynamic_pointer_cast<chrono::ChLinkEngine>(this->ChData);
		if (auto mfun = std::dynamic_pointer_cast<chrono::ChFunction_Const>(engine->Get_rot_funct()))
			mfun->Set_yconst(motion);
	}
	else if (EngineType == EEngineMode::ENG_MODE_SPEED) {
		auto engine = std::dynamic_pointer_cast<chrono::ChLinkEngine>(this->ChData);
		if (auto mfun = std::dynamic_pointer_cast<chrono::ChFunction_Const>(engine->Get_spe_funct()))
			mfun->Set_yconst(motion);
	}
	else if (EngineType == EEngineMode::ENG_MODE_TORQUE) {
		//UE_LOG(LogTemp, Warning, TEXT("ENG_MODE_TORQUE"));
		//UE_LOG(LogTemp, Warning, TEXT("bUseEngineCurve %d"), bUseEngineCurve);

		if (bUseEngineCurve && Curve) {
			auto engine = std::dynamic_pointer_cast<chrono::ChLinkEngine>(this->ChData);
			float angularSpeed = abs(engine->Get_mot_rot_dt());
			auto torqueCurve = Curve->GetFloatValue(angularSpeed);
			//UE_LOG(LogTemp, Warning, TEXT("AngularSpeed %f"), engine->Get_mot_rot_dt());
			//UE_LOG(LogTemp, Warning, TEXT("Torque %f"), torqueCurve);
			//UE_LOG(LogTemp, Warning, TEXT("Motion %f"), this->motion);
			if (auto mfun = std::dynamic_pointer_cast<chrono::ChFunction_Const>(engine->Get_tor_funct()))
				mfun->Set_yconst(this->motion * torqueCurve);
			//UE_LOG(LogTemp, Warning, TEXT("Torque %f"), this->motion * torqueCurve);
			engineTorque = this->motion * torqueCurve;
		}
		else {
			//UE_LOG(LogTemp, Warning, TEXT("No Curve Mode"));
			auto engine = std::dynamic_pointer_cast<chrono::ChLinkEngine>(this->ChData);
			if (auto mfun = std::dynamic_pointer_cast<chrono::ChFunction_Const>(engine->Get_tor_funct()))
				mfun->Set_yconst(this->motion);
			engineTorque = motion;
		}
	}

	if (this->bShowMessage) {
		ShowDebugMessage();
	}
}

void AChLink_EngineActor::SetMotion(float motion)
{
	this->motion = motion;
}

void AChLink_EngineActor::AddMotion(float motion)
{
	this->motion += motion;
}

FExportData AChLink_EngineActor::ExportData()
{
	FExportData data = Super::ExportData();

	if (bExportData) {
		if (bExportEngineSpeed) {
			data.floatData.Add(FName("EngineSpeed"), std::dynamic_pointer_cast<chrono::ChLinkEngine>(this->ChData)->Get_mot_rot_dt());
		}
		if (bExportEngineTorque) {
			data.floatData.Add(FName("EngineTorque"), this->engineTorque);
		}
	}

	return data;
}

void AChLink_EngineActor::ShowDebugMessage()
{
	UE_LOG(LogTemp, Warning, TEXT("AChLink_EngineActor::ShowDebugMessage()"));
	if (isInitialized) {
		auto torque = this->ChData->Get_react_torque();
		auto FT = CHRONO_VEC_TO_FVECTOR(torque) / 100.f;
		UE_LOG(LogTemp, Warning, TEXT("Engine Torque %s"), *FT.ToString());
	}
}
