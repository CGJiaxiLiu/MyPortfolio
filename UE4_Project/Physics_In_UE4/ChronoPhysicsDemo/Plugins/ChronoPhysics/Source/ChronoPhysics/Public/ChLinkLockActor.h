// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChLinkMarkersActor.h"
#include "ChLinkLockActor.generated.h"

UENUM()
namespace ELinkLockType {
	enum Type {
		LOCK,
		SPHERICAL,
		POINTPLANE,
		POINTLINE,
		CYLINDRICAL,
		PRISMATIC,
		PLANEPLANE,
		OLDHAM,
		REVOLUTE,
		FREE,
		ALIGN,
		PARALLEL,
		PERPEND,
		TRAJECTORY,
		CLEARANCE,
		REVOLUTEPRISMATIC
	};
}

UCLASS()
class CHRONOPHYSICS_API AChLinkLockActor : public AChLinkMarkersActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "Chrono|LinkType")
	TEnumAsByte<ELinkLockType::Type> LinkLockType;

	UPROPERTY(EditAnywhere, Category = "Chrono|LinkLimit", meta = (EditConditionToggle))
	uint8 bUseLimitX : 1;
	UPROPERTY(EditAnywhere, Category = "Chrono|LinkLimit", meta = (DisplayName = "X max", editcondition = "bUseLimitX"))
	float XLimitMax;
	UPROPERTY(EditAnywhere, Category = "Chrono|LinkLimit", meta = (DisplayName = "X min", editcondition = "bUseLimitX"))
	float XLimitMin;
	UPROPERTY(EditAnywhere, Category = "Chrono|LinkLimit", meta = (EditConditionToggle))
	uint8 bUseLimitY : 1;
	UPROPERTY(EditAnywhere, Category = "Chrono|LinkLimit", meta = (DisplayName = "Y max", editcondition = "bUseLimitY"))
	float YLimitMax;
	UPROPERTY(EditAnywhere, Category = "Chrono|LinkLimit", meta = (DisplayName = "Y min", editcondition = "bUseLimitY"))
	float YLimitMin;
	UPROPERTY(EditAnywhere, Category = "Chrono|LinkLimit", meta = (EditConditionToggle))
	uint8 bUseLimitZ : 1;
	UPROPERTY(EditAnywhere, Category = "Chrono|LinkLimit", meta = (DisplayName = "Z max", editcondition = "bUseLimitZ"))
	float ZLimitMax;
	UPROPERTY(EditAnywhere, Category = "Chrono|LinkLimit", meta = (DisplayName = "Z min", editcondition = "bUseLimitZ"))
	float ZLimitMin;
	UPROPERTY(EditAnywhere, Category = "Chrono|LinkLimit", meta = (EditConditionToggle))
	uint8 bUseLimitRx : 1;
	UPROPERTY(EditAnywhere, Category = "Chrono|LinkLimit", meta = (DisplayName = "Rx max", editcondition = "bUseLimitRx"))
	float RxLimitMax;
	UPROPERTY(EditAnywhere, Category = "Chrono|LinkLimit", meta = (DisplayName = "Rx min", editcondition = "bUseLimitRx"))
	float RxLimitMin;
	UPROPERTY(EditAnywhere, Category = "Chrono|LinkLimit", meta = (EditConditionToggle))
	uint8 bUseLimitRy : 1;
	UPROPERTY(EditAnywhere, Category = "Chrono|LinkLimit", meta = (DisplayName = "Ry max", editcondition = "bUseLimitRy"))
	float RyLimitMax;
	UPROPERTY(EditAnywhere, Category = "Chrono|LinkLimit", meta = (DisplayName = "Ry min", editcondition = "bUseLimitRy"))
	float RyLimitMin;
	UPROPERTY(EditAnywhere, Category = "Chrono|LinkLimit", meta = (EditConditionToggle))
	uint8 bUseLimitRz : 1;
	UPROPERTY(EditAnywhere, Category = "Chrono|LinkLimit", meta = (DisplayName = "Rz max", editcondition = "bUseLimitRz"))
	float RzLimitMax;
	UPROPERTY(EditAnywhere, Category = "Chrono|LinkLimit", meta = (DisplayName = "Rz min", editcondition = "bUseLimitRz"))
	float RzLimitMin;

	virtual void PhysicsObjectConstruct() override;
	virtual void ChLinkInitialize() override;

};
