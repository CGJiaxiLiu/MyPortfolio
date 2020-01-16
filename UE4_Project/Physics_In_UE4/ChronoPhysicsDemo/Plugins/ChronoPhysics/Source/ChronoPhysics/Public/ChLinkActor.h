// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ChPhysicsObjectInterface.h"
#include <memory>
#include "ChLinkActor.generated.h"

namespace chrono {
	class ChLink;
}

UCLASS()
class CHRONOPHYSICS_API AChLinkActor : public AActor, public IChPhysicsObjectInterface
{
	GENERATED_BODY()
	
public:	

	UPROPERTY(VisibleInstanceOnly)
	class UStaticMeshComponent* Arrow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chrono")
	AActor* target1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chrono")
	AActor* target2;

	UPROPERTY(EditAnywhere, Category = "Chrono|ExportData", meta = (EditConditionToggle))
	bool bExportData = false;

	UPROPERTY(EditAnywhere, Category = "Chrono|ExportData", meta = (editcondition = "bExportData"))
	FName LinkExportDataOwnerName;

	UPROPERTY(EditAnywhere, Category = "Chrono|ExportData", meta = (editcondition = "bExportData"))
	bool bExportReactForce = false;

	UPROPERTY(EditAnywhere, Category = "Chrono|ExportData", meta = (editcondition = "bExportData"))
	bool bExportReactTorque = false;

	AChLinkActor();

	virtual void PhysicsObjectConstruct() override;
	virtual void PhysicsObjectInitalize() override;
	virtual void AddToSystem(std::shared_ptr<chrono::ChSystem> phySystem) override;
	virtual void UpdatePhysicsState() override {}
	virtual void UpdateVisualAsset() override {}
	virtual FExportData ExportData() override;
	virtual bool& GetIsExportData() override { return bExportData; }
	virtual FName& GetExportDataOwnerName() override { return LinkExportDataOwnerName; }

	bool IsReadyForInitialize();
	virtual void ChLinkInitialize() {}

	std::shared_ptr<chrono::ChLink> ChData;

	

};
