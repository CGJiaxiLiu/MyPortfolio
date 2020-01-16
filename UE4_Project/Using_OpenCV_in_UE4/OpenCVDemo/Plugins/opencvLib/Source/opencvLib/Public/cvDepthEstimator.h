// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/CoreUObject/Public/UObject/UObjectGlobals.h"
#include "Engine.h"
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"	
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/opencv.hpp"
#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Engine/Texture2D.h"
#include "Runtime/Engine/Classes/Engine/Texture.h"
#include "Runtime/Core/Public/Misc/Paths.h"
#include "Runtime/Core/Public/Misc/FileHelper.h"
#include "Runtime/ImageWrapper/Public/IImageWrapper.h" 
#include "Runtime/ImageWrapper/Public/IImageWrapperModule.h"  
#include "Runtime/Core/Public/Templates/SharedPointer.h"
#include "Runtime/Engine/Public/UnrealClient.h"
#include "Runtime/Engine/Classes/Materials/Material.h"
#include "Runtime/Engine/Public/SceneTypes.h"
#include "Runtime/RHI/Public/RHIDefinitions.h"
#include "Runtime/CoreUObject/Public/Serialization/BulkData.h"
#include "Runtime/Engine/Classes/Engine/TextureRenderTarget2D.h"
#include "Runtime/CoreUObject/Public/UObject/ObjectMacros.h"
#include "Runtime/Engine/Classes/Engine/GameViewportClient.h"
#include "Runtime/Engine/Public/UnrealClient.h"
#include "Runtime/Engine/Public/TextureResource.h"
#include "Runtime/RHI/Public/RHI.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Engine/Public/SceneInterface.h"
#include "Runtime/Engine/Classes/Components/LightComponent.h"
#include "Runtime/Engine/Classes/Engine/SceneCapture2D.h"
#include "cvDepthEstimator.generated.h"


UCLASS()
class OPENCVLIB_API AcvDepthEstimator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AcvDepthEstimator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	//UTexture2D* LoadTexture2D(const FString& ImagePath, bool& IsValid, int32& OutWidth, int32& OutHeight);
	//UTexture2D* GetTexture2DFromDiskFile(const FString& FilePath , bool& isvalid,int&a ,int& b);
	//TSharedPtr<IImageWrapper> GetImageWrapperByExtention(const FString InImagePath);



	// OpenCV fields
	cv::Mat frame,gray,frame_2;
	//cv::VideoCapture stream=0;
	//cv::Size size;
	cv::Mat dstImage, normImage, scaledImage, dual, realDepth;

	//cv dual camera
	//**************************************
	int blockSize = 2, uniquenessRatio = 0, numDisparities = 0;
	cv::Ptr<cv::StereoBM> bm = cv::StereoBM::create(9, 9);
	cv::Mat disp, disp8, grayImageL, grayImageR;

	int sum = 0;

	UPROPERTY(Editanywhere, BlueprintReadWrite)
		float threshold=220;

	UPROPERTY(Editanywhere, BlueprintReadWrite)
		int maxpoint = 10;


	//read from gamethread rendertarget
	//*************************************
	TArray<FColor> ImageData_1, ImageData_2, ImageData_3;

	FTextureRenderTargetResource* RenderTarget_1;
	FTextureRenderTargetResource* RenderTarget_2;
	FTextureRenderTargetResource* RenderTarget_3;

	int32 TargetX_1, TargetY_1, TargetX_2, TargetY_2, TargetX_3, TargetY_3;

	UPROPERTY(Editanywhere,  Category = Webcam)
	UTextureRenderTarget2D *SceneCapture_1;

	UPROPERTY(Editanywhere,  Category = Webcam)
	UTextureRenderTarget2D *SceneCapture_2;

	UPROPERTY(Editanywhere, Category = Webcam)
	UTextureRenderTarget2D *SceneCapture_3;


	//read from viewport
	//****************************
	FViewport* myviewport;
	int32 viewportX, viewportY;
	TArray<FColor> ViewportImage;
	cv::Mat viewportframe,Model, similarity,showsimilarity,temp;
	cv::Rect ViewportBox, searchWindow;
	cv::Point point;
	double mag_r=0,s_min,s_max;

	FIntPoint MouseIni,MouseCur,MouseFinal;

public:
	//void OnMouseClickedBegin();
	//void OnMouseClickedEnd();

	bool isClicked = false,bBeginTrack = false;


	//recogenize circle
	//***********************************
	cv::Mat edges;
	std::vector<cv::Vec3f> circles;


};
	
	

