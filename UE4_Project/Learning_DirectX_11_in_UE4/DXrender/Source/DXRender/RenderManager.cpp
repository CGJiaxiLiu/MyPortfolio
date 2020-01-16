// Fill out your copyright notice in the Description page of Project Settings.


#include "RenderManager.h"
#include "D3DManager.h"
#include "VisualDisplayer.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "PolygonData.h"
#include "ShaderManager.h"
#include "Engine/DirectionalLight.h"
#include "Components/LightComponent.h"
#include "Engine/StaticMesh.h"
#include "EngineUtils.h"
#include "Runtime/Engine/Public/StaticMeshResources.h"
#include "Materials/MaterialInterface.h"


// Sets default values
ARenderManager::ARenderManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARenderManager::BeginPlay()
{
	Super::BeginPlay();

	if (m_target) {
		m_D3D = new D3DManager();
		m_Displayer = new VisualDisplayer();
		m_ShaderManager = new ShaderManager();
		m_PolygonData = new PolygonData();

		bD3DInitialized = m_D3D->Initialize(m_target->SizeX, m_target->SizeY);
		m_Displayer->SetOutputTarget(m_target);

		if (bD3DInitialized) {

			m_Displayer->SetD3DManager(m_D3D);
			bDisplayerInitialized = m_Displayer->Initialize();

			TActorIterator<AActor> actorItr = TActorIterator<AActor>(GetWorld());

			for (actorItr; actorItr; ++actorItr) {
				if (actorItr->ActorHasTag(FName("Render"))) {
					for (auto compPtr : actorItr->GetComponents()) {
						UStaticMeshComponent* meshComp = Cast<UStaticMeshComponent>(compPtr);
						if (meshComp) {
							UE_LOG(LogTemp, Warning, TEXT("Actor Name: %s"), *actorItr->GetHumanReadableName());
							auto mesh = meshComp->GetStaticMesh();
							this->m_meshArr.Add(mesh);
							if (mesh->RenderData->LODResources.Num() > 0) {
								m_PolygonData->AddGeometryData(&mesh->RenderData->LODResources[0]);
							}
						}
					}
				}
			}
			m_PolygonData->SetD3DManager(m_D3D);
			bPolygonDataInitialzied = m_PolygonData->Initialize();

			if (bPolygonDataInitialzied) {
				FString VS = FPaths::Combine(FPaths::ProjectDir(), FString("Shader"), FString("color.vs"));
				FString PS = FPaths::Combine(FPaths::ProjectDir(), FString("Shader"), FString("color.ps"));

				m_ShaderManager->SetD3DManager(m_D3D);
				m_ShaderManager->SetVSFileName(VS);
				m_ShaderManager->SetPSFileName(PS);
				m_ShaderManager->SetPolygonData(m_PolygonData);

				m_ShaderManager->m_textureInMaterial = m_texture;

				bRenderManagerInitialzied = m_ShaderManager->Initialize();
			}
		}
	}

	m_cameraManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
	if (m_cameraManager) {
		bCameraInitialized = true;
	}

	EverythingInitialized = bD3DInitialized && bRenderManagerInitialzied && bCameraInitialized;
}

void ARenderManager::BeginDestroy()
{
	Super::BeginDestroy();

	if (m_D3D) {
		m_D3D->Destroy();
		delete m_D3D;
		m_D3D = 0;
	}
	if (m_Displayer) {
		m_Displayer->Destroy();
		delete m_Displayer;
		m_Displayer = 0;
	}
	if (m_PolygonData) {
		m_PolygonData->Destroy();
		delete m_PolygonData;
		m_PolygonData = 0;
	}
	if (m_ShaderManager) {
		m_ShaderManager->Destroy();
		delete m_ShaderManager;
		m_ShaderManager = 0;
	}
}

// Called every frame
void ARenderManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (EverythingInitialized) {

		D3DXMATRIX viewMatrix, projectionMatrix, worldMatrix;
		D3DXMatrixPerspectiveFovLH(&projectionMatrix, m_cameraManager->GetCameraCachePOV().FOV * 0.0174532925f, (float)m_target->SizeX /(float)m_target->SizeY, 0.1f, 100.0f);
		D3DXMatrixIdentity(&worldMatrix);

		D3DXVECTOR3 up, position, lookAt;
		float yaw, pitch, roll;
		D3DXMATRIX rotationMatrix;
		up.x = 0.0f;
		up.y = 1.0f;
		up.z = 0.0f;

		position.x = m_cameraManager->GetCameraCachePOV().Location.Y / 100.0f;
		position.y = m_cameraManager->GetCameraCachePOV().Location.Z / 100.0f;
		position.z = m_cameraManager->GetCameraCachePOV().Location.X / 100.0f;

		lookAt.x = 0.0f;
		lookAt.y = 0.0f;
		lookAt.z = 1.0f;

		pitch = m_cameraManager->GetCameraCachePOV().Rotation.Pitch * -0.0174532925f;
		yaw = m_cameraManager->GetCameraCachePOV().Rotation.Yaw * 0.0174532925f;
		roll = m_cameraManager->GetCameraCachePOV().Rotation.Roll * 0.0174532925f;

		D3DXMatrixRotationYawPitchRoll(&rotationMatrix, yaw, pitch, roll);
		D3DXVec3TransformCoord(&lookAt, &lookAt, &rotationMatrix);
		D3DXVec3TransformCoord(&up, &up, &rotationMatrix);
		lookAt = position + lookAt;
		D3DXMatrixLookAtLH(&viewMatrix, &position, &lookAt, &up);


		D3DXMatrixTranspose(&worldMatrix, &worldMatrix);
		D3DXMatrixTranspose(&viewMatrix, &viewMatrix);
		D3DXMatrixTranspose(&projectionMatrix, &projectionMatrix);

		m_D3D->SetRenderToTexture();
		m_D3D->DXBeginScene(GetGameTimeSinceCreation());
		m_PolygonData->Update();

		m_ShaderManager->VSConstBuffer->projection = projectionMatrix;
		m_ShaderManager->VSConstBuffer->view = viewMatrix;
		m_ShaderManager->VSConstBuffer->world = worldMatrix;
		//m_ShaderManager->PSConstBuffer->intensity = sin(3.0f * GetGameTimeSinceCreation());
		m_ShaderManager->PSConstBuffer->intensity = 1.0f;
		m_ShaderManager->PSConstBuffer->lightDirection = D3DXVECTOR3(1.0f, 0.2f, 0.3f);
		m_ShaderManager->VSConstBuffer->viewPosition.x = m_cameraManager->GetCameraLocation().Y / 100.0f;
		m_ShaderManager->VSConstBuffer->viewPosition.y = m_cameraManager->GetCameraLocation().Z / 100.0f;
		m_ShaderManager->VSConstBuffer->viewPosition.z = m_cameraManager->GetCameraLocation().X / 100.0f;
		m_ShaderManager->PSConstBuffer->specularPower = 2.5f;

		if (m_directionalLight) {
			FVector direc = m_directionalLight->GetActorRotation().RotateVector(FVector(1.0f, 0.0f, 0.0f));
			m_ShaderManager->PSConstBuffer->lightDirection = D3DXVECTOR3(direc.Y, direc.Z, direc.X);

			auto light = Cast<ULightComponent>(m_directionalLight->GetComponentByClass(ULightComponent::StaticClass()));
			if (light) {
				m_ShaderManager->PSConstBuffer->intensity = light->Intensity / 3.1415926f;
			}
		}

		m_ShaderManager->UpdateShaderParameters();
		m_ShaderManager->Render();

		m_D3D->TurnOnAlphaBlend();

		D3DXMatrixTranslation(&worldMatrix, 0.7f, 1.0f, 0.1f);
		D3DXMatrixTranspose(&worldMatrix, &worldMatrix);
		m_ShaderManager->VSConstBuffer->world = worldMatrix;
		m_ShaderManager->UpdateShaderParameters();
		m_ShaderManager->Render();

		D3DXMatrixTranslation(&worldMatrix, 0.7f, 1.0f, 3.0f);
		D3DXMATRIX rot;
		D3DXMatrixRotationX(&rot, 45);
		D3DXMatrixMultiply(&worldMatrix, &rot, &worldMatrix);
		D3DXMatrixTranspose(&worldMatrix, &worldMatrix);
		m_ShaderManager->VSConstBuffer->world = worldMatrix;
		m_ShaderManager->UpdateShaderParameters();
		m_ShaderManager->Render();

		m_D3D->TurnOffAlphaBlend();
		m_Displayer->DisplayOnTexture();


#ifdef RENDER_IN_WINDOW

		m_Displayer->SetRenderToWindow();

		float color[4];

		// Setup the color to clear the buffer to.
		color[2] = 0.5f;
		color[0] = 0.3f;
		color[1] = 0.1f;
		color[3] = 1.0f;

		m_D3D->GetDeviceContext()->ClearRenderTargetView(m_Displayer->GetWindowRenderTargetView(), color);
		m_D3D->GetDeviceContext()->ClearDepthStencilView(m_D3D->GetDepthStencilView(), D3D11_CLEAR_DEPTH, 1.0f, 0);
		m_PolygonData->Update();
		m_ShaderManager->Render();
		m_Displayer->DisplayOnWindow();

#endif // RENDER_IN_WINDOW

	}
}

