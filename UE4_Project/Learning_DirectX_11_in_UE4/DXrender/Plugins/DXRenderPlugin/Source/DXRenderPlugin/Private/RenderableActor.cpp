// Fill out your copyright notice in the Description page of Project Settings.


#include "RenderableActor.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Runtime/Engine/Public/StaticMeshResources.h"
#include "Engine/Texture2D.h"

// Sets default values
ARenderableActor::ARenderableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent0"));
	StaticMeshComponent->Mobility = EComponentMobility::Movable;
	StaticMeshComponent->SetGenerateOverlapEvents(false);
	StaticMeshComponent->bUseDefaultCollision = true;

	RootComponent = StaticMeshComponent;
}

FStaticMeshLODResources * ARenderableActor::GetGeometryData()
{
	if (auto mesh = this->StaticMeshComponent->GetStaticMesh()) {
		if (mesh->RenderData->LODResources.Num() > 0) {
			return &mesh->RenderData->LODResources[0];
		}
	}

	return nullptr;
}

D3DXVECTOR3 ARenderableActor::GetDXPosition()
{
	auto pos =  D3DXVECTOR3();
	pos.x = this->GetActorLocation().Y / 100.0f;
	pos.y = this->GetActorLocation().Z / 100.0f;
	pos.z = this->GetActorLocation().X / 100.0f;
	return pos;
}

D3DXVECTOR3 ARenderableActor::GetDXRotation()
{
	auto rot = D3DXVECTOR3();
	rot.x = this->GetActorRotation().Yaw * 0.0174532925f;
	rot.y = this->GetActorRotation().Pitch * -0.0174532925f;
	rot.z = this->GetActorRotation().Roll * 0.0174532925f;
	return rot;
}

D3DXVECTOR3 ARenderableActor::GetDXScale()
{
	auto scale = D3DXVECTOR3();
	scale.x = this->GetActorScale().Y;
	scale.y = this->GetActorScale().Z;
	scale.z = this->GetActorScale().X;
	return scale;
}

TArray<uint8> ARenderableActor::GetTextureSourceData()
{
	auto data = TArray<uint8>();

	if (!Texture) {
		return data;
	}

#undef UpdateResource

	TextureCompressionSettings OldCompressionSettings = Texture->CompressionSettings;
	TextureMipGenSettings OldMipGenSettings = Texture->MipGenSettings;
	bool OldSRGB = Texture->SRGB;

	Texture->CompressionSettings = TextureCompressionSettings::TC_VectorDisplacementmap;
	Texture->MipGenSettings = TextureMipGenSettings::TMGS_NoMipmaps;
	Texture->SRGB = false;
	Texture->UpdateResource();

	if (Cast<UTexture2D>(Texture)) {
		auto tex2D = Cast<UTexture2D>(Texture);
		if (auto dataPtr = tex2D->PlatformData->Mips[0].BulkData.LockReadOnly()) {
			int sizeX = tex2D->PlatformData->Mips[0].SizeX;
			int sizeY = tex2D->PlatformData->Mips[0].SizeY;
			data.Init(0, sizeX * sizeY * 4);
			memcpy(data.GetData(), dataPtr, sizeX * sizeY * 4);
			tex2D->PlatformData->Mips[0].BulkData.Unlock();
		}
	}

	Texture->CompressionSettings = OldCompressionSettings;
	Texture->MipGenSettings = OldMipGenSettings;
	Texture->SRGB = OldSRGB;
	Texture->UpdateResource();

	return data;
}

