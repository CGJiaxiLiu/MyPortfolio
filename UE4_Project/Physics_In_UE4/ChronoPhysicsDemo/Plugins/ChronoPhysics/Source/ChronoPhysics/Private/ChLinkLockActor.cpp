// Fill out your copyright notice in the Description page of Project Settings.


#include "ChLinkLockActor.h"
#include "chrono/physics/ChLinkLock.h"
#include "ChBodyComponent.h"
#include "util.h"

#define TypeCast()

void AChLinkLockActor::PhysicsObjectConstruct()
{
	auto linkptr = std::make_shared<chrono::ChLinkLock>();

	switch (LinkLockType) {

	case ELinkLockType::FREE:
		linkptr->ChangeLinkType(chrono::ChLinkLock::LinkType::FREE);
		break;
	case ELinkLockType::LOCK:
		linkptr->ChangeLinkType(chrono::ChLinkLock::LinkType::LOCK);
		break;
	case ELinkLockType::SPHERICAL:
		linkptr->ChangeLinkType(chrono::ChLinkLock::LinkType::SPHERICAL);
		break;
	case ELinkLockType::POINTPLANE:
		linkptr->ChangeLinkType(chrono::ChLinkLock::LinkType::POINTPLANE);
		break;
	case ELinkLockType::POINTLINE:
		linkptr->ChangeLinkType(chrono::ChLinkLock::LinkType::POINTLINE);
		break;
	case ELinkLockType::REVOLUTE:
		linkptr->ChangeLinkType(chrono::ChLinkLock::LinkType::REVOLUTE);
		break;
	case ELinkLockType::CYLINDRICAL:
		linkptr->ChangeLinkType(chrono::ChLinkLock::LinkType::CYLINDRICAL);
		break;
	case ELinkLockType::PRISMATIC:
		linkptr->ChangeLinkType(chrono::ChLinkLock::LinkType::PRISMATIC);
		break;
	case ELinkLockType::PLANEPLANE:
		linkptr->ChangeLinkType(chrono::ChLinkLock::LinkType::PLANEPLANE);
		break;
	case ELinkLockType::OLDHAM:
		linkptr->ChangeLinkType(chrono::ChLinkLock::LinkType::OLDHAM);
		break;
	case ELinkLockType::ALIGN:
		linkptr->ChangeLinkType(chrono::ChLinkLock::LinkType::ALIGN);
		break;
	case ELinkLockType::PARALLEL:
		linkptr->ChangeLinkType(chrono::ChLinkLock::LinkType::PARALLEL);
		break;
	case ELinkLockType::PERPEND:
		linkptr->ChangeLinkType(chrono::ChLinkLock::LinkType::PERPEND);
		break;
	case ELinkLockType::REVOLUTEPRISMATIC:
		linkptr->ChangeLinkType(chrono::ChLinkLock::LinkType::REVOLUTEPRISMATIC);
		break;
	default:
		linkptr->ChangeLinkType(chrono::ChLinkLock::LinkType::FREE);
		break;
	}
	this->ChData = linkptr;
}

void AChLinkLockActor::ChLinkInitialize()
{
	Super::ChLinkInitialize();
	if (this->isInitialized) {
		this->isInitialized = false;

		if (std::dynamic_pointer_cast<chrono::ChLinkLock>(ChData)) {
			if (bUseLimitX) {
				std::dynamic_pointer_cast<chrono::ChLinkLock>(ChData)->GetLimit_X()->Set_active(true);
				std::dynamic_pointer_cast<chrono::ChLinkLock>(ChData)->GetLimit_X()->Set_max(XLimitMax);
				std::dynamic_pointer_cast<chrono::ChLinkLock>(ChData)->GetLimit_X()->Set_min(XLimitMin);
			}
			if (bUseLimitY) {
				std::dynamic_pointer_cast<chrono::ChLinkLock>(ChData)->GetLimit_Y()->Set_active(true);
				std::dynamic_pointer_cast<chrono::ChLinkLock>(ChData)->GetLimit_Y()->Set_max(YLimitMax);
				std::dynamic_pointer_cast<chrono::ChLinkLock>(ChData)->GetLimit_Y()->Set_min(YLimitMin);
			}
			if (bUseLimitZ) {
				std::dynamic_pointer_cast<chrono::ChLinkLock>(ChData)->GetLimit_Z()->Set_active(true);
				std::dynamic_pointer_cast<chrono::ChLinkLock>(ChData)->GetLimit_Z()->Set_max(ZLimitMax);
				std::dynamic_pointer_cast<chrono::ChLinkLock>(ChData)->GetLimit_Z()->Set_min(ZLimitMin);
			}
			if (bUseLimitRx) {
				std::dynamic_pointer_cast<chrono::ChLinkLock>(ChData)->GetLimit_Rx()->Set_active(true);
				std::dynamic_pointer_cast<chrono::ChLinkLock>(ChData)->GetLimit_Rx()->Set_max(RxLimitMax * 0.0174532925);
				std::dynamic_pointer_cast<chrono::ChLinkLock>(ChData)->GetLimit_Rx()->Set_min(RxLimitMin * 0.0174532925);
			}
			if (bUseLimitRy) {
				std::dynamic_pointer_cast<chrono::ChLinkLock>(ChData)->GetLimit_Ry()->Set_active(true);
				std::dynamic_pointer_cast<chrono::ChLinkLock>(ChData)->GetLimit_Ry()->Set_max(RyLimitMax * 0.0174532925);
				std::dynamic_pointer_cast<chrono::ChLinkLock>(ChData)->GetLimit_Ry()->Set_min(RyLimitMin * 0.0174532925);
			}
			if (bUseLimitRz) {
				std::dynamic_pointer_cast<chrono::ChLinkLock>(ChData)->GetLimit_Rz()->Set_active(true);
				std::dynamic_pointer_cast<chrono::ChLinkLock>(ChData)->GetLimit_Rz()->Set_max(RzLimitMax * 0.0174532925);
				std::dynamic_pointer_cast<chrono::ChLinkLock>(ChData)->GetLimit_Rz()->Set_min(RzLimitMin * 0.0174532925);
			}

			this->isInitialized = true;
		}
	}
}
