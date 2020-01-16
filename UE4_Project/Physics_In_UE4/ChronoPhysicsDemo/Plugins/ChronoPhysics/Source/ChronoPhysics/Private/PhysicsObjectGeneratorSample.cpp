// Fill out your copyright notice in the Description page of Project Settings.


#include "PhysicsObjectGeneratorSample.h"

void APhysicsObjectGeneratorSample::PhysicsObjectConstruct()
{
	TArray<AChBody_GeneratedActor*> boxList;
	for (int i = 0; i < BoxCount; i++) {
		auto box = NewChBodyActor(EShapeType::Box);
		box->ChComp->PhysicsObjectConstruct();
		boxList.Add(box);
		box->SetActorLocation(FVector(i * 50, i * 50, -i * 150));

		if (i == 0) {
			box->ChComp->isFixed = true;
		}
		else {
			box->ChComp->isFixed = false;
			auto link = NewChLinkActor(ELinkType::SPHERICAL);
			link->PhysicsObjectConstruct();
			link->target1 = box;
			link->target2 = boxList[i - 1];
			link->SetActorLocation((box->GetActorLocation() + boxList[i - 1]->GetActorLocation()) / 2);
		}
	}
}
