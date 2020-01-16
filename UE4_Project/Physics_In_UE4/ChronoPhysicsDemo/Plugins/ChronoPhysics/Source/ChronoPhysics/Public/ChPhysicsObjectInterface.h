#pragma once

#include "UObject/Interface.h"
#include <memory>
#include "ChPhysicsObjectInterface.generated.h"

namespace chrono {
	class ChSystem;
}

USTRUCT(BlueprintType)
struct FExportData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName onwerName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FName, FVector> data;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FName, float> floatData;

};

UINTERFACE()
class UChPhysicsObjectInterface : public UInterface
{
	GENERATED_BODY()
};

class IChPhysicsObjectInterface
{
	GENERATED_BODY()

public:
	virtual void PhysicsObjectConstruct() = 0;
	virtual void PhysicsObjectInitalize() = 0;
	virtual void AddToSystem(std::shared_ptr<chrono::ChSystem> phySystem) = 0;
	virtual void AddToSystem(TArray<TScriptInterface<IChPhysicsObjectInterface>>& objList) {}
	virtual void UpdatePhysicsState() = 0;
	virtual void UpdateVisualAsset() = 0;
	virtual bool IsBody() { return false; }
	virtual bool& GetIsForParallel() { return mute; }
	virtual FExportData ExportData() { return FExportData(); }
	virtual bool& GetIsExportData() { return bExportDataInterface; }
	virtual FName& GetExportDataOwnerName() { return DataOnwnerNameInterface; }


protected:
	bool isInitialized;
	bool mute;
	bool bExportDataInterface = false;
	FName DataOnwnerNameInterface;
};
