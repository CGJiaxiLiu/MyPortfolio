#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ChPhysicsObjectInterface.h"
#include <memory>
#include "ChBodyComponent.generated.h"


namespace chrono {
	class ChBody;
}


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CHRONOPHYSICS_API UChBodyComponent : public UActorComponent, public IChPhysicsObjectInterface
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = "Chrono|BodyBasicProperty")
	bool isFixed = false;

	UPROPERTY(EditAnywhere, Category = "Chrono|BodyBasicProperty")
	bool isForParallel = false;

	UPROPERTY(EditAnywhere, Category = "Chrono|BodyBasicProperty")
	double Density = 1000;

	UPROPERTY(EditAnywhere, Category = "Chrono|Collision")
	bool isCollide = true;

	UPROPERTY(EditAnywhere, Category = "Chrono|Collision")
	int CollisionFamily = -1;

	UPROPERTY(EditAnywhere, Category = "Chrono|Collision")
	TArray<int> NoCollisionWithFamiliy;

	UPROPERTY(EditAnywhere, Category = "Chrono|DynamicPreset")
	float MaxSpeed = 1000.0f;

	UPROPERTY(EditAnywhere, Category = "Chrono|DynamicPreset")
	float MaxAngularSpeed = 100.0f;

	UPROPERTY(EditAnywhere, Category = "Chrono|DynamicPreset", meta = (EditConditionToggle))
	bool bAddCustomForce = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chrono|DynamicPreset", meta = (editcondition = "bAddCustomForce"))
	FVector CustomForce;

	UPROPERTY(EditAnywhere, Category = "Chrono|DynamicPreset", meta = (EditConditionToggle))
	bool bUseDrag = false;

	UPROPERTY(EditAnywhere, Category = "Chrono|DynamicPreset", meta = (editcondition = "bUseDrag"))
	float DragCoefTrans = 0.001f;

	UPROPERTY(EditAnywhere, Category = "Chrono|DynamicPreset", meta = (editcondition = "bUseDrag"))
	float DragCoefRot = 0.001f;

	UPROPERTY(EditAnywhere, Category = "Chrono|MaterialParameter")
	float StaticFriction = 0.55f;

	UPROPERTY(EditAnywhere, Category = "Chrono|MaterialParameter")
	float SlidingFriction = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Chrono|MaterialParameter")
	float RollingFriction = 0.f;

	UPROPERTY(EditAnywhere, Category = "Chrono|MaterialParameter")
	float SpinningFriction = 0.f;

	UPROPERTY(EditAnywhere, Category = "Chrono|MaterialParameter")
	float Cohension = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Chrono|MaterialParameter")
	float Restitution = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Chrono|MaterialParameter")
	float DampingF = 0.2f;

	UPROPERTY(EditAnywhere, Category = "Chrono|MaterialParameter")
	float Compliance = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Chrono|MaterialParameter")
	float ComplianceT = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Chrono|MaterialParameter")
	float ComplianceRoll = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Chrono|MaterialParameter")
	float ComplianceSpin = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Chrono|ExportData", meta = (EditConditionToggle))
	bool bExportData = false;

	UPROPERTY(EditAnywhere, Category = "Chrono|ExportData", meta = (editcondition = "bExportData"))
	FName BodyExportDataOwnerName;

	UPROPERTY(EditAnywhere, Category = "Chrono|ExportData", meta = (editcondition = "bExportData"))
	bool bExportPos = false;

	UPROPERTY(EditAnywhere, Category = "Chrono|ExportData", meta = (editcondition = "bExportData"))
	bool bExportVel = false;

	UPROPERTY(EditAnywhere, Category = "Chrono|ExportData", meta = (editcondition = "bExportData"))
	bool bExportAcce = false;

	UPROPERTY(EditAnywhere, Category = "Chrono|ExportData", meta = (editcondition = "bExportData"))
	bool bExportAngVel = false;

	UPROPERTY(EditAnywhere, Category = "Chrono|ExportData", meta = (editcondition = "bExportData"))
	bool bExportContactForce = false;

public:	
	UChBodyComponent();

	virtual void PhysicsObjectConstruct() override;
	virtual void PhysicsObjectInitalize() override;
	virtual void AddToSystem(std::shared_ptr<chrono::ChSystem> phySystem) override;
	virtual void UpdatePhysicsState() override;
	virtual void UpdateVisualAsset() override;
	FORCEINLINE std::shared_ptr<chrono::ChBody> GetChData() { return this->ChData; }
	virtual bool IsBody() override { return true; }
	virtual bool& GetIsForParallel() override { return isForParallel; }
	virtual FExportData ExportData() override;
	virtual bool& GetIsExportData() override { return bExportData; }
	virtual FName& GetExportDataOwnerName() override { return BodyExportDataOwnerName; }

protected:
	std::shared_ptr<chrono::ChBody> ChData;

};
