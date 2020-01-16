// Fill out your copyright notice in the Description page of Project Settings.


#include "ChPhysicsSceneManagerActor.h"
#include "chrono/physics/ChSystemNSC.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "chrono/collision/ChCCollisionSystemBullet.h"
#include "ChPhysicsObjectInterface.h"
#include "ChBodyComponent.h"
#include "chrono/physics/ChContactContainer.h"
#include "DrawDebugHelpers.h"
#include "util.h"
#include "chrono_vehicle/terrain/SCMDeformableTerrain.h"
#include "ChBody_GeneratedActor.h"

AChPhysicsSceneManagerActor::AChPhysicsSceneManagerActor()
{
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AChPhysicsSceneManagerActor::BeginPlay()
{
	Super::BeginPlay();

	SystemInitialize();
	FetchPhysicsObject();
	InitPhysicsObject();
	AddObjectToSystem();
}

// Called every frame
void AChPhysicsSceneManagerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	StepPhysics(DeltaTime);

	for (auto body : this->PhysicsObjectList) {
		body->UpdateVisualAsset();
	}
}

void AChPhysicsSceneManagerActor::SystemInitialize()
{
	this->phySystem = std::make_shared<chrono::ChSystemNSC>();
	//phySystem->SetSolverType(chrono::ChSolver::Type::SOR);
	//phySystem->SetMaxItersSolverSpeed(100);
	//phySystem->SetSolverType(chrono::ChSolver::Type::APGD);
	//phySystem->SetSolverType(chrono::ChSolver::Type::BARZILAIBORWEIN);
	//phySystem->SetSolverType(chrono::ChSolver::Type::JACOBI);
	//phySystem->SetTimestepperType(chrono::ChTimestepper::Type::EULER_IMPLICIT_PROJECTED);

	phySystem->SetMaxItersSolverSpeed(MaxItersSolverSpeed);
	phySystem->SetMaxItersSolverStab(MaxItersSolverStab);

	if (bSetDefaultCollisionParameter) {
		chrono::collision::ChCollisionModel::SetDefaultSuggestedEnvelope(DefaultSuggestedEnvelope);
		chrono::collision::ChCollisionModel::SetDefaultSuggestedMargin(DefaultSuggestedMargin);
		chrono::collision::ChCollisionSystemBullet::SetContactBreakingThreshold(SetContactBreakingThreshold);
	}


}

void AChPhysicsSceneManagerActor::FetchPhysicsObject()
{
	TActorIterator<AActor> actorItr = TActorIterator<AActor>(GetWorld());

	for (actorItr; actorItr; ++actorItr) {
		IChPhysicsObjectInterface* phyObject = Cast<IChPhysicsObjectInterface>(*actorItr);
		if (phyObject) {
			this->PhysicsObjectList.Add(*actorItr);
		}
		else {
			for (auto compPtr : actorItr->GetComponents()) {
				IChPhysicsObjectInterface* phyObject = Cast<IChPhysicsObjectInterface>(compPtr);
				if (phyObject) {
					this->PhysicsObjectList.Add(compPtr);
				}
			}
		}
	}
}

void AChPhysicsSceneManagerActor::InitPhysicsObject()
{
	for (auto Obj : this->PhysicsObjectList) {
		Obj->PhysicsObjectConstruct();
		//Obj->PhysicsObjectInitalize();
	}

	for (auto Obj : this->PhysicsObjectList) {
		//Obj->PhysicsObjectConstruct();
		Obj->PhysicsObjectInitalize();
	}

	//for (auto Obj : this->PhysicsObjectList) {
	//	Obj->PhysicsObjectConstruct();
	//	//Obj->PhysicsObjectInitalize();
	//}

	//for (auto Obj : this->PhysicsObjectList) {
	//	//Obj->PhysicsObjectConstruct();
	//	Obj->PhysicsObjectInitalize();
	//}
}

void AChPhysicsSceneManagerActor::AddObjectToSystem()
{
	int size = PhysicsObjectList.Num();
	
	for (int i = 0; i < size; i++) {
		PhysicsObjectList[i]->AddToSystem(this->phySystem);
		PhysicsObjectList[i]->AddToSystem(this->PhysicsObjectList);
	}

	float mass = 0;
	for (auto obj : phySystem->Get_bodylist()) {
		if (!obj->GetBodyFixed()) {
			mass += obj->GetMass();
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Total Mass: %f"), mass);
}

void AChPhysicsSceneManagerActor::StepPhysics(float deltaTime)
{
	for (int i = 0; i < Substep; i++) {
		this->phySystem->DoStepDynamics(deltaTime / Substep <= MaxStepLengthms / 1000 ? deltaTime / Substep : MaxStepLengthms / 1000);

		for (auto body : this->PhysicsObjectList) {
			body->UpdatePhysicsState();
		}
	}
}

const TMap<FName, FExportData> AChPhysicsSceneManagerActor::ExportData()
{
	TMap<FName, FExportData> data;

	for (auto obj : PhysicsObjectList) {
		if (obj->GetIsExportData()) {
			data.Add(obj->GetExportDataOwnerName(), obj->ExportData());
		}
	}

	return data;
}

class saveContactDataCallback : public chrono::ChContactContainer::ReportContactCallback
{
public:
	std::vector<chrono::ChVector<>> forceList;
	std::vector<chrono::ChVector<>> forcePosList;
	std::vector<chrono::ChMatrix33<>> forceRotList;
	std::vector<chrono::ChContactable*> contactObjList;

public:
	virtual bool OnReportContact(const chrono::ChVector<>& pA,
		const chrono::ChVector<>& pB,
		const chrono::ChMatrix33<>& plane_coord,
		const double& distance,
		const double& eff_Radius,
		const chrono::ChVector<>& react_forces,
		const chrono::ChVector<>& react_torques,
		chrono::ChContactable* modA,
		chrono::ChContactable* modB) override {

		forcePosList.push_back((pA + pB) / 2.0f);
		forceList.push_back(react_forces);
		forceRotList.push_back(plane_coord);
		contactObjList.push_back(modA);
		contactObjList.push_back(modB);
		return true;  // to continue scanning contacts
	}

	virtual void clearAllData() {
		forceList.clear();
		forcePosList.clear();
		contactObjList.clear();
	}
};

//TArray<FVector> AChPhysicsSceneManagerActor::ReportContactDataWithSpecifyBody(AActor * specifyBody, ATrackActor * vehicle)
//{
//	saveContactDataCallback contactData;
//	contactData.clearAllData();
//	TArray<FVector> trackPos;
//	TArray<float> distance;
//	distance.Init(0, 4);
//	TArray<FVector> trackForce;
//	trackForce.Init(FVector(0, 0, 0), 4);
//	for (auto track : vehicle->Tracks) {
//		trackPos.Add(CHRONO_VEC_TO_FVECTOR(track->Truss->ChComp->GetChData()->GetPos()));
//	}
//
//	UChBodyComponent* comp = Cast<UChBodyComponent>(specifyBody->GetComponentByClass(UChBodyComponent::StaticClass()));
//	if (!comp) {
//		return trackForce;
//	}
//
//	phySystem->GetContactContainer()->ReportAllContacts(&contactData);
//	int num = 0;
//	auto bodyPtr = comp->GetChData();
//
//	FVector forceSum;
//	for (int i = 0; i < contactData.forcePosList.size(); i++) {
//		if (std::dynamic_pointer_cast<chrono::ChContactable>(bodyPtr).get() == contactData.contactObjList[2 * i] || std::dynamic_pointer_cast<chrono::ChContactable>(bodyPtr).get() == contactData.contactObjList[2 * i + 1]) {
//			num++;
//			FVector start = CHRONO_VEC_TO_FVECTOR((contactData.forcePosList[i]));
//			FVector force_original = CHRONO_VEC_TO_FVECTOR((contactData.forceList[i])) / 100.0f;
//			
//			if (force_original.Size() >= 0.5) {
//				FVector force = CHRONO_VEC_TO_FVECTOR((contactData.forceRotList[i].Get_A_quaternion().Rotate(contactData.forceList[i]))) / 100.0f;
//				if (force.Z <= 0) {
//					force *= -1;
//				}
//				//force.RotateAngleAxis(-90, FVector(0, 1, 0));
//				//UE_LOG(LogTemp, Warning, TEXT("Force_Original %s"), *force_original.ToString());
//				//UE_LOG(LogTemp, Warning, TEXT("Force %s"), *force.ToString());
//				FVector end = start + force;
//				//FVector end = start + FVector(0, 0, 100);
//				DrawDebugLine(GetWorld(), start, end, FColor::Red, false, -1.0f, 0, 1.0f);
//				forceSum += force;
//
//				for (int i = 0; i < 4; i++) {
//					distance[i] = (trackPos[i] - start).Size();
//				}
//				for (int i = 0; i < 4; i++) {
//					bool isMinimum = true;
//
//					for (int j = 1; j < 4; j++) {
//						if (distance[i] > distance[(i + j) % 4]) {
//							isMinimum = false;
//							break;
//						}
//					}
//					if (isMinimum) {
//						trackForce[i] += force;
//					}
//				}
//			}
//		}
//	}
//
//
//	//UE_LOG(LogTemp, Warning, TEXT("Sum Force%s"), *forceSum.ToString());
//
//	return trackForce;
//	//else if (Cast<UChBody_SCMTerrainComponent>(specifyBody->GetComponentByClass(UChBody_SCMTerrainComponent::StaticClass()))) {
//	//	//UE_LOG(LogTemp, Warning, TEXT("SCM Terrain"));
//	//	auto terrain = Cast<UChBody_SCMTerrainComponent>(specifyBody->GetComponentByClass(UChBody_SCMTerrainComponent::StaticClass()))->ChDataTerrain;
//	//	FVector forcePoint = CHRONO_VEC_TO_FVECTOR((terrain->GetContactForce(phySystem->Get_bodylist()[15]).force));
//	//	UE_LOG(LogTemp, Warning, TEXT("SCM Force Point:%s"), *forcePoint.ToString());
//	//}
//}
