// Fill out your copyright notice in the Description page of Project Settings.


#include "RenderManagerActor.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "RenderableActor.h"
#include "Util.h"
#include "DXManager.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Displayer.h"
#include "GeometryDataManager.h"
#include "ShaderManager.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Engine/DirectionalLight.h"
#include "Components/LightComponent.h"

// Sets default values
ARenderManagerActor::ARenderManagerActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARenderManagerActor::BeginPlay()
{
	Super::BeginPlay();
	FetchRenderableActor();
	LOG("Renderable Actor Count: %d", aRenderableActor.Num());

	if (pRenderTarget) {
		int32 width = pRenderTarget->SizeX;
		int32 height = pRenderTarget->SizeY;

		pDXManager = new DXManager();
		bInitialized = pDXManager->Initialize(width, height) && bInitialized;

		pDisplayer = new Displayer();
		bInitialized = pDisplayer->Initialize(width, height, pRenderTarget) && bInitialized;
		if (bInitialized) {
			pDXManager->SetRenderTargetView(pDisplayer->GetRenderTargetView());
		}

		pGeoDataManager = new GeometryDataManager();
		for (auto actor : aRenderableActor) {
			pGeoDataManager->AddGeometryData(actor->GetGeometryData(), actor->Texture);
		}
		bInitialized = pGeoDataManager->Intialize() && bInitialized;

		pShaderManager = new ShaderManager();
		FString VS = FPaths::Combine(FPaths::ProjectDir(), FString("Shader"), FString("color.vs"));
		FString PS = FPaths::Combine(FPaths::ProjectDir(), FString("Shader"), FString("color.ps"));
		bInitialized = pShaderManager->Initialize(VS, PS) && bInitialized;


		pCameraManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
		if (pCameraManager) {
			bCameraInitialized = true;
		}
	}
	else {
		bInitialized = false;
		LOG("Missing Render Target Texture");
	}
}

void ARenderManagerActor::BeginDestroy()
{
	Super::BeginDestroy();

	if (pDXManager) {
		pDXManager->Destroy();
	}
	if (pDisplayer) {
		pDisplayer->Destroy();
	}
	if (pGeoDataManager) {
		pGeoDataManager->Destroy();
	}
	if (pShaderManager) {
		pShaderManager->Destroy();
	}
}

void ARenderManagerActor::FetchRenderableActor()
{
	TActorIterator<ARenderableActor> actorItr = TActorIterator<ARenderableActor>(GetWorld());

	for (actorItr; actorItr; ++actorItr) {
		aRenderableActor.Add(*actorItr);
	}
}

// Called every frame
void ARenderManagerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bInitialized) {
		return;
	}

	pDXManager->InitializeScene(0.2f, 0.2f, 0.2f, 1.0f);

	D3DXMATRIX viewMatrix, projectionMatrix;

	D3DXMatrixPerspectiveFovLH(&projectionMatrix, pCameraManager->GetCameraCachePOV().FOV * 0.0174532925f, (float)pRenderTarget->SizeX / (float)pRenderTarget->SizeY, 0.1f, 100.0f);
	D3DXVECTOR3 up, position, lookAt;
	float yaw, pitch, roll;
	D3DXMATRIX rotationMatrix;
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	position.x = pCameraManager->GetCameraCachePOV().Location.Y / 100.0f;
	position.y = pCameraManager->GetCameraCachePOV().Location.Z / 100.0f;
	position.z = pCameraManager->GetCameraCachePOV().Location.X / 100.0f;

	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	pitch = pCameraManager->GetCameraCachePOV().Rotation.Pitch * -0.0174532925f;
	yaw = pCameraManager->GetCameraCachePOV().Rotation.Yaw * 0.0174532925f;
	roll = pCameraManager->GetCameraCachePOV().Rotation.Roll * 0.0174532925f;

	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, yaw, pitch, roll);
	D3DXVec3TransformCoord(&lookAt, &lookAt, &rotationMatrix);
	D3DXVec3TransformCoord(&up, &up, &rotationMatrix);
	lookAt = position + lookAt;
	D3DXMatrixLookAtLH(&viewMatrix, &position, &lookAt, &up);

	D3DXMatrixTranspose(&viewMatrix, &viewMatrix);
	D3DXMatrixTranspose(&projectionMatrix, &projectionMatrix);

	pShaderManager->VSConstBuffer->view = viewMatrix;
	pShaderManager->VSConstBuffer->projection = projectionMatrix;

	pShaderManager->PSConstBuffer->intensity = 1.0f;
	pShaderManager->PSConstBuffer->lightDirection = D3DXVECTOR3(1.0f, 0.2f, 0.3f);
	pShaderManager->VSConstBuffer->viewPosition.x = pCameraManager->GetCameraLocation().Y / 100.0f;
	pShaderManager->VSConstBuffer->viewPosition.y = pCameraManager->GetCameraLocation().Z / 100.0f;
	pShaderManager->VSConstBuffer->viewPosition.z = pCameraManager->GetCameraLocation().X / 100.0f;
	pShaderManager->PSConstBuffer->specularPower = 2.5f;

	if (pDirectionalLight) {
		auto rotator = pDirectionalLight->GetActorRotation();
		//rotator.Pitch *= -1.0f;
		FVector direc = rotator.RotateVector(FVector(1.0f, 0.0f, 0.0f));
		pShaderManager->PSConstBuffer->lightDirection = D3DXVECTOR3(direc.Y, direc.Z, direc.X);

		auto light = Cast<ULightComponent>(pDirectionalLight->GetComponentByClass(ULightComponent::StaticClass()));
		if (light) {
			pShaderManager->PSConstBuffer->intensity = light->Intensity / 3.1415926f;
		}
	}

	for (int i = 0; i < pGeoDataManager->GetObjectCount(); i++) {
		D3DXMATRIX worldMatrix;
		D3DXMATRIX inversedWorldMatrix;
		float _x = aRenderableActor[i]->GetDXPosition().x;
		float _y = aRenderableActor[i]->GetDXPosition().y;
		float _z = aRenderableActor[i]->GetDXPosition().z;
		D3DXMatrixTranslation(&worldMatrix, _x, _y, _z);

		D3DXMATRIX rot;
		float _yaw = aRenderableActor[i]->GetDXRotation().x;
		float _pitch = aRenderableActor[i]->GetDXRotation().y;
		float _roll = aRenderableActor[i]->GetDXRotation().z;
		D3DXMatrixRotationYawPitchRoll(&rot, _yaw, _pitch, -_roll);

		D3DXMATRIX scale;
		float scaleX = aRenderableActor[i]->GetDXScale().x;
		float scaleY = aRenderableActor[i]->GetDXScale().y;
		float scaleZ = aRenderableActor[i]->GetDXScale().z;
		D3DXMatrixScaling(&scale, scaleX, scaleY, scaleZ);

		D3DXMatrixMultiply(&rot, &scale, &rot);
		D3DXMatrixMultiply(&worldMatrix, &rot, &worldMatrix);
		D3DXMatrixInverse(&inversedWorldMatrix, nullptr, &worldMatrix);
		D3DXMatrixTranspose(&worldMatrix, &worldMatrix);
		//D3DXMatrixTranspose(&inversedWorldMatrix, &inversedWorldMatrix);

		pShaderManager->VSConstBuffer->world = worldMatrix;
		pShaderManager->VSConstBuffer->inversedWorld = inversedWorldMatrix;
		pShaderManager->UpdateShaderParameters();

		uint32 start = pGeoDataManager->GetMetaData(i)->startIndex;
		uint32 length = pGeoDataManager->GetMetaData(i)->length;
		pShaderManager->Render(length, start);
	}

	pDisplayer->Display();
}

