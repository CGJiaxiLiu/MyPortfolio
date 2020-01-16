// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Windows/MinWindows.h"
#include <d3d11.h>
#include <d3dx10math.h>
#include "RenderableActor.generated.h"

UCLASS()
class DXRENDERPLUGIN_API ARenderableActor : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, Category = "DXRender", DisplayName = "Mesh")
	class UStaticMeshComponent* StaticMeshComponent;

public:	
	UPROPERTY(EditAnywhere, Category = "DXRender", DisplayName = "Texture")
	class UTexture2D* Texture;

	// Sets default values for this actor's properties
	ARenderableActor();
	struct FStaticMeshLODResources* GetGeometryData();
	D3DXVECTOR3 GetDXPosition();

	// Return Yaw Pitch Roll
	D3DXVECTOR3 GetDXRotation();

	// Return ScaleX ScaleY ScaleZ
	D3DXVECTOR3 GetDXScale();

	TArray<uint8> GetTextureSourceData();
};
