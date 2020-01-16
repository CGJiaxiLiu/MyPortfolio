// Fill out your copyright notice in the Description page of Project Settings.


#include "ChLink_SpringActor.h"
#include "chrono/physics/ChLinkSpring.h"

void AChLink_SpringActor::PhysicsObjectConstruct()
{
	this->ChData = std::make_shared<chrono::ChLinkSpring>();
}

void AChLink_SpringActor::ChLinkInitialize()
{
	Super::ChLinkInitialize();
	if (std::dynamic_pointer_cast<chrono::ChLinkSpring>(ChData)) {
		std::dynamic_pointer_cast<chrono::ChLinkSpring>(ChData)->Set_SpringK(SpringCoef);
		std::dynamic_pointer_cast<chrono::ChLinkSpring>(ChData)->Set_SpringR(DampCoef);
		this->isInitialized = true;
	}
}
