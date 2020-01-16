// Fill out your copyright notice in the Description page of Project Settings.


#include "PhysicsObjectGeneratorBasis.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "ChBody_BoxComponent.h"
#include "ChBody_CylinderComponent.h"
#include "ChBody_SphereComponent.h"

// Sets default values
APhysicsObjectGeneratorBasis::APhysicsObjectGeneratorBasis()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ArrowMesh(TEXT("/ChronoPhysics/BasicShapeMesh/Arrow.Arrow"));

	VisualAsset = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Arrow"));
	VisualAsset->SetStaticMesh(ArrowMesh.Object);
	VisualAsset->CastShadow = false;
	VisualAsset->bHiddenInGame = true;
	RootComponent = VisualAsset;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> boxFinder(TEXT("/ChronoPhysics/BasicShapeMesh/SM_Box.SM_Box"));
	boxMesh = boxFinder.Object;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> cylinderFinder(TEXT("/ChronoPhysics/BasicShapeMesh/SM_Cylinder.SM_Cylinder"));
	cylinderMesh = cylinderFinder.Object;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> sphereFinder(TEXT("/ChronoPhysics/BasicShapeMesh/SM_Sphere.SM_Sphere"));
	sphereMesh = sphereFinder.Object;
}

void APhysicsObjectGeneratorBasis::PhysicsObjectInitalize()
{
	for (auto obj : PhysicsObjectList) {
		obj->PhysicsObjectInitalize();
	}
}

void APhysicsObjectGeneratorBasis::AddToSystem(std::shared_ptr<chrono::ChSystem> phySystem)
{
	for (auto obj : PhysicsObjectList) {
		obj->AddToSystem(phySystem);
	}
}

void APhysicsObjectGeneratorBasis::UpdatePhysicsState()
{
	for (auto obj : PhysicsObjectList) {
		obj->UpdatePhysicsState();
	}
}

void APhysicsObjectGeneratorBasis::UpdateVisualAsset()
{
	for (auto obj : PhysicsObjectList) {
		obj->UpdateVisualAsset();
	}
}

bool APhysicsObjectGeneratorBasis::SetEngineMotion(int index, float motion)
{
	if (EngineList.Num() > index) {
		EngineList[index]->SetMotion(motion);
		return true;
	}
	return false;
}

float APhysicsObjectGeneratorBasis::GetEngineMotion(int index)
{
	if (EngineList.Num() > index) {
		return EngineList[index]->GetMotion();
	}
	return 99999999.f;
}

AChBody_GeneratedActor * APhysicsObjectGeneratorBasis::NewChBodyActor(EShapeType::Type shape)
{
	auto newBody = GetWorld()->SpawnActor<AChBody_GeneratedActor>(AChBody_GeneratedActor::StaticClass());

	UChBodyComponent* newComp;
	if (shape == EShapeType::Box) {
		Cast<UStaticMeshComponent>(newBody->GetRootComponent())->SetStaticMesh(boxMesh);
		newComp = NewObject<UChBody_BoxComponent>(newBody, TEXT("ChComp"));
		newComp->RegisterComponent();
		newBody->ChComp = newComp;
	}
	else if (shape == EShapeType::Cylinder) {
		Cast<UStaticMeshComponent>(newBody->GetRootComponent())->SetStaticMesh(cylinderMesh);
		newComp = NewObject<UChBody_CylinderComponent>(newBody, TEXT("ChComp"));
		newComp->RegisterComponent();
		newBody->ChComp = newComp;
	}
	else if (shape == EShapeType::Sphere) {
		Cast<UStaticMeshComponent>(newBody->GetRootComponent())->SetStaticMesh(sphereMesh);
		newComp = NewObject<UChBody_SphereComponent>(newBody, TEXT("ChComp"));
		newComp->RegisterComponent();
		newBody->ChComp = newComp;
	}

	PhysicsObjectList.Add(newBody->ChComp);
	return newBody;
}

AChLinkActor * APhysicsObjectGeneratorBasis::NewChLinkActor(ELinkType::Type linkType)
{
	if (linkType == ELinkType::ENGINE_ROTATION || linkType == ELinkType::ENGINE_SPEED || linkType == ELinkType::ENGINE_TORQUE) {
		auto engine = GetWorld()->SpawnActor<AChLink_EngineActor>(AChLink_EngineActor::StaticClass());
		switch (linkType) {
		case ELinkType::ENGINE_ROTATION:
			engine->EngineType = EEngineMode::ENG_MODE_ROTATION;
		case ELinkType::ENGINE_SPEED:
			engine->EngineType = EEngineMode::ENG_MODE_SPEED;
		case ELinkType::ENGINE_TORQUE:
			engine->EngineType = EEngineMode::ENG_MODE_TORQUE;
		default:
			engine->EngineType = EEngineMode::ENG_MODE_TORQUE;
		}
		PhysicsObjectList.Add(engine);
		EngineList.Add(engine);
		return engine;
	}
	else {
		auto newActor = GetWorld()->SpawnActor<AChLinkLockActor>(AChLinkLockActor::StaticClass());
		ELinkLockType::Type linkLockType;
		switch (linkType) {
		case ELinkType::FREE:
			linkLockType = ELinkLockType::FREE;
			break;
		case ELinkType::LOCK:
			linkLockType = ELinkLockType::LOCK;
			break;
		case ELinkType::SPHERICAL:
			linkLockType = ELinkLockType::SPHERICAL;
			break;
		case ELinkType::POINTPLANE:
			linkLockType = ELinkLockType::POINTPLANE;
			break;
		case ELinkType::POINTLINE:
			linkLockType = ELinkLockType::POINTLINE;
			break;
		case ELinkType::REVOLUTE:
			linkLockType = ELinkLockType::REVOLUTE;
			break;
		case ELinkType::CYLINDRICAL:
			linkLockType = ELinkLockType::CYLINDRICAL;
			break;
		case ELinkType::PRISMATIC:
			linkLockType = ELinkLockType::PRISMATIC;
			break;
		case ELinkType::PLANEPLANE:
			linkLockType = ELinkLockType::PLANEPLANE;
			break;
		case ELinkType::OLDHAM:
			linkLockType = ELinkLockType::OLDHAM;
			break;
		case ELinkType::ALIGN:
			linkLockType = ELinkLockType::ALIGN;
			break;
		case ELinkType::PARALLEL:
			linkLockType = ELinkLockType::PARALLEL;
			break;
		case ELinkType::PERPEND:
			linkLockType = ELinkLockType::PERPEND;
			break;
		case ELinkType::REVOLUTEPRISMATIC:
			linkLockType = ELinkLockType::REVOLUTEPRISMATIC;
			break;
		default:
			linkLockType = ELinkLockType::FREE;
			break;
		}
		newActor->LinkLockType = linkLockType;
		PhysicsObjectList.Add(newActor);
		return newActor;
	}
}

