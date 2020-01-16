// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RenderManagerActor.generated.h"

UCLASS()
class DXRENDERPLUGIN_API ARenderManagerActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARenderManagerActor();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;

	void FetchRenderableActor();
	TArray<class ARenderableActor *> aRenderableActor;

	UPROPERTY(EditAnywhere, Category = "DXRender", DisplayName = "Render Target")
	class UTextureRenderTarget2D* pRenderTarget;

	UPROPERTY(EditAnywhere, Category = "DXRender", DisplayName = "Directional Light")
	class ADirectionalLight* pDirectionalLight;

	class APlayerCameraManager* pCameraManager = 0;

	class DXManager * pDXManager = 0;
	class Displayer * pDisplayer = 0;
	class GeometryDataManager * pGeoDataManager = 0;
	class ShaderManager * pShaderManager = 0;

	bool bInitialized = true;
	bool bCameraInitialized = false;
};
