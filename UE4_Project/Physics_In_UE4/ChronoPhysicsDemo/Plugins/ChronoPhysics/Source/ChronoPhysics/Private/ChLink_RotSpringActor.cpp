// Fill out your copyright notice in the Description page of Project Settings.


#include "ChLink_RotSpringActor.h"

void AChLink_RotSpringActor::PhysicsObjectConstruct()
{
	this->ChData = std::make_shared<chrono::ChLinkRotSpringCB>();
	this->myTorqueFuntor = MakeShared<MyTorque>(DampCoef, SpringCoef);
}

void AChLink_RotSpringActor::ChLinkInitialize()
{
	Super::ChLinkInitialize();

	if (std::dynamic_pointer_cast<chrono::ChLinkRotSpringCB>(ChData)) {
		std::dynamic_pointer_cast<chrono::ChLinkRotSpringCB>(ChData)->RegisterTorqueFunctor(myTorqueFuntor.Get());
		
		this->isInitialized = true;
	}

}
