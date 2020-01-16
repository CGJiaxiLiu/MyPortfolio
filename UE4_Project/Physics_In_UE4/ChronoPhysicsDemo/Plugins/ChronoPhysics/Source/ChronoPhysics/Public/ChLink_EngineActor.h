// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChLinkLockActor.h"
#include "ChLink_EngineActor.generated.h"

UENUM()
namespace EEngineMode {
	enum Type {
		ENG_MODE_ROTATION,
		ENG_MODE_SPEED,
		ENG_MODE_TORQUE,
	};
}

UCLASS()
class CHRONOPHYSICS_API AChLink_EngineActor : public AChLinkLockActor
{
	GENERATED_BODY()

public:
	AChLink_EngineActor();

	UPROPERTY(EditAnywhere, Category = "Chrono|EngineType")
	bool bShowMessage = false;

	UPROPERTY(EditAnywhere, Category = "Chrono|EngineType")
	TEnumAsByte<EEngineMode::Type> EngineType;

	UPROPERTY(EditAnywhere, Category = "Chrono|EngineCurve", meta = (EditConditionToggle))
	bool bUseEngineCurve = false;

	UPROPERTY(EditDefaultsOnly, Category = "Chrono|EngineCurve", meta = (editcondition = "bUseEngineCurve"))
	class UCurveFloat* Curve;

	UPROPERTY(EditAnywhere, Category = "Chrono|ExportData", meta = (editcondition = "bExportData"))
	bool bExportEngineSpeed = false;

	UPROPERTY(EditAnywhere, Category = "Chrono|ExportData", meta = (editcondition = "bExportData"))
	bool bExportEngineTorque = false;

	virtual void PhysicsObjectConstruct() override;
	virtual void UpdatePhysicsState() override;
	UFUNCTION(BlueprintCallable, Category = "Chrono")
	void SetMotion(float motion);
	UFUNCTION(BlueprintCallable, Category = "Chrono")
	void AddMotion(float motion);
	UFUNCTION(BlueprintPure, Category = "Chrono")
	float GetMotion() { return motion; }
	virtual FExportData ExportData() override;

	void ShowDebugMessage();

private:
	float motion;
	float engineTorque;


};
