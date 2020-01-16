// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RenderManager.generated.h"

UCLASS()
class DXRENDER_API ARenderManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARenderManager();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "DXRender", DisplayName = "Render Target")
	class UTextureRenderTarget2D* m_target;

	UPROPERTY(EditAnywhere, Category = "DXRender", DisplayName = "Directional Light")
	class ADirectionalLight* m_directionalLight;

	UPROPERTY(EditAnywhere, Category = "DXRender", DisplayName = "Texture")
	class UTexture2D* m_texture;

protected:
	class D3DManager* m_D3D = 0;
	class VisualDisplayer* m_Displayer = 0;
	class ShaderManager* m_ShaderManager = 0;
	class PolygonData* m_PolygonData = 0;
	class APlayerCameraManager* m_cameraManager = 0;
	TArray<class UStaticMesh*> m_meshArr;

	bool bD3DInitialized = false;
	bool bDisplayerInitialized = false;
	bool bPolygonDataInitialzied = false;
	bool bRenderManagerInitialzied = false;
	bool bCameraInitialized = false;
	bool EverythingInitialized = false;
};
