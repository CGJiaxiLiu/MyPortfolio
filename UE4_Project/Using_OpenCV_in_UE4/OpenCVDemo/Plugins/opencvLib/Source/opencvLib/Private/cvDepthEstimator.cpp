// Fill out your copyright notice in the Description page of Project Settings.

#include "cvDepthEstimator.h"


// Sets default values
AcvDepthEstimator::AcvDepthEstimator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}

// Called when the game starts or when spawned
void AcvDepthEstimator::BeginPlay()
{
	Super::BeginPlay();

	GEngine->AddOnScreenDebugMessage(-1, 8.0f, FColor::Yellow, FString("WebCamera Begin Play"));
	
	//camera_1 initial
	RenderTarget_1 = SceneCapture_1->GameThread_GetRenderTargetResource();
	TargetX_1 = SceneCapture_1->SizeX;
	TargetY_1 = SceneCapture_1->SizeY;
	RenderTarget_1->ReadPixels(ImageData_1);
	frame.create(TargetY_1, TargetX_1, CV_8UC(3));


	//camera_2 initial
	RenderTarget_2 = SceneCapture_2->GameThread_GetRenderTargetResource();
	TargetX_2 = SceneCapture_1->SizeX;
	TargetY_2 = SceneCapture_1->SizeY;
	RenderTarget_2->ReadPixels(ImageData_2);
	frame_2.create(TargetY_2, TargetX_2, CV_8UC(3));

	//camera_2 initial
	RenderTarget_3 = SceneCapture_3->GameThread_GetRenderTargetResource();
	TargetX_3 = SceneCapture_1->SizeX;
	TargetY_3 = SceneCapture_1->SizeY;
	RenderTarget_3->ReadPixels(ImageData_3);
	realDepth.create(TargetY_3, TargetX_3, CV_8UC(3));
	
	for (int y = 0; y < TargetY_1; y++)
	{
		for (int x = 0; x <TargetX_1; x++)
		{
			int i = x + (y * TargetX_1);
			frame.data[i * 3 + 0] = ImageData_1[i].B;
			frame.data[i * 3 + 1] = ImageData_1[i].G;
			frame.data[i * 3 + 2] = ImageData_1[i].R;
			frame_2.data[i * 3 + 0] = ImageData_2[i].B;
			frame_2.data[i * 3 + 1] = ImageData_2[i].G;
			frame_2.data[i * 3 + 2] = ImageData_2[i].R;
			realDepth.data[i * 3 + 0] = ImageData_3[i].B;
			realDepth.data[i * 3 + 1] = ImageData_3[i].G;
			realDepth.data[i * 3 + 2] = ImageData_3[i].R;
		}
	}

	//cv dual camera initial

	cv::cvtColor(frame, grayImageL, CV_BGR2GRAY);
	cv::cvtColor(frame_2, grayImageR, CV_BGR2GRAY);

	//using for detect circle
	bm->setBlockSize(2 * blockSize + 5);     //SAD窗口大小，5~21之间为宜
	//bm->setROI1(validROIL);
	//bm->setROI2(validROIR);
	bm->setPreFilterCap(31);
	bm->setMinDisparity(0);  //最小视差，默认值为0, 可以是负值，int型
	bm->setNumDisparities(numDisparities * 16 + 16);//视差窗口，即最大视差值与最小视差值之差,窗口大小必须是16的整数倍，int型
	bm->setTextureThreshold(10);
	bm->setUniquenessRatio(uniquenessRatio);//uniquenessRatio主要可以防止误匹配
	bm->setSpeckleWindowSize(100);
	bm->setSpeckleRange(32);
	bm->setDisp12MaxDiff(-1);
	//bm->compute(grayImageL, grayImageR, disp);//输入图像必须为灰度图
	//disp.convertTo(disp8, CV_8U, 255 / ((numDisparities * 16 + 16)*16.));//计算出的视差是CV_16S格式
	//reprojectImageTo3D(disp, xyz, Q, true); //在实际求距离时，ReprojectTo3D出来的X / W, Y / W, Z / W都要乘以16(也就是W除以16)，才能得到正确的三维坐标信息。
	//xyz = xyz * 16;

}
//void AcvDepthEstimator::OnMouseClickedBegin()
//{
//	isClicked = true;
//	myviewport->GetMousePos(MouseIni);
//}

//void AcvDepthEstimator::OnMouseClickedEnd()
//{
//	isClicked = false;
//	Model = viewportframe(ViewportBox);
//	cv::cvtColor(Model, Model, cv::COLOR_BGR2GRAY);
//	bBeginTrack = true;
//	searchWindow.width = ViewportBox.width;
//	searchWindow.height = ViewportBox.height;
//}



// Called every frame
void AcvDepthEstimator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RenderTarget_1->ReadPixels(ImageData_1);
	RenderTarget_2->ReadPixels(ImageData_2);
	RenderTarget_3->ReadPixels(ImageData_3);

	for (int y = 0; y < TargetY_1; y++)
	{
		for (int x = 0; x <TargetX_1; x++)
		{
			int i = x + (y * TargetX_1);
			frame.data[i * 3 + 0] = ImageData_1[i].B;
			frame.data[i * 3 + 1] = ImageData_1[i].G;
			frame.data[i * 3 + 2] = ImageData_1[i].R;
			frame_2.data[i * 3 + 0] = ImageData_2[i].B;
			frame_2.data[i * 3 + 1] = ImageData_2[i].G;
			frame_2.data[i * 3 + 2] = ImageData_2[i].R;
			realDepth.data[i * 3 + 0] = ImageData_3[i].B;
			realDepth.data[i * 3 + 1] = ImageData_3[i].G;
			realDepth.data[i * 3 + 2] = ImageData_3[i].R;
		}
	}

	//dual = (frame + frame_2) / 2;
	//cv::GaussianBlur(frame, frame, cv::Size(3, 3), 0, 0);
	//cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
	//dstImage = cv::Mat::zeros(frame.size(), CV_32FC1);
	//cornerHarris(gray, dstImage, 2, 3, 0.04);  //进行角点检测
	//normalize(dstImage, normImage, 0, 255, cv::NORM_MINMAX);  //归一化
	//convertScaleAbs(normImage, scaledImage);  //转换为8位无符号整形
	//										  //将检测到的，且符合阈值条件的角点绘制出来
	//for (int j = 5; j < normImage.rows-5; j++)
	//{
	//	for (int i = 5; i < normImage.cols-5; i++)
	//	{
	//		if ((int)normImage.at<float>(j, i)>threshold)
	//		{
	//			cv::circle(frame, cv::Point(i, j), 5, cv::Scalar(10, 10, 255), 2);
	//			sum++;
	//			if (sum >= maxpoint)
	//				goto flag;

	//		}
	//	}
	//}

//flag:
	sum = 0;
	cv::imshow("frame",frame);
	cv::imshow("frame2", frame_2);

	cv::cvtColor(frame, grayImageL, CV_BGR2GRAY);
	cv::cvtColor(frame_2, grayImageR, CV_BGR2GRAY);

	bm->compute(grayImageL, grayImageR, disp);
	//disp.convertTo(disp8, CV_8U, 255 / ((numDisparities * 16 + 16)*16.));
	//disp = 2500 / disp;
	disp.convertTo(disp8, CV_8U, 255 / ((numDisparities * 16 + 16)*16.));
	imshow("disparity", disp8);
	imshow("realDepth", realDepth);


	//cv::GaussianBlur(grayImageL, edges, cv::Size(37, 37), 2, 2);
	//cv::HoughCircles(edges, circles, CV_HOUGH_GRADIENT, 1.5, 10, 200, 100, 0, 0);
	//for (uint8 i = 0; i < circles.size(); i++)
	//{
	//	cv::Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
	//	int radius = cvRound(circles[i][2]);
	//	//绘制圆心  
	//	cv::circle(frame, center, 3, cv::Scalar(0, 255, 0), -1, 8, 0);
	//	//绘制圆轮廓  
	//	cv::circle(frame, center, radius, cv::Scalar(155, 50, 255), 3, 8, 0);
	//}

	//imshow("circle", frame);


	//read from render target

//	texture2d = MyTarget2d->ConstructTexture2D(this, FString("RenderTex"), EObjectFlags::RF_NoFlags, CTF_DeferCompression);
//	texture2d->CompressionSettings = TextureCompressionSettings::TC_VectorDisplacementmap;
//	texture2d->SRGB = 0;
//	texture2d->UpdateResource();
//	texture2d->PlatformData->Mips[0].BulkData.GetCopy((void**)&FormatedImageData);
//	GEngine->AddOnScreenDebugMessage(-1, 8.0f, FColor::Black, FString::FromInt(FormatedImageData[15].G));
//
//	FormatedImageData2 = static_cast<FColor*>(RawImageData->Lock(LOCK_READ_ONLY));
//	RawImageData->Unlock();
//
//	
//	for (int y = 0; y < texture2d->GetSizeX(); y++)
//		{
//			for (int x = 0; x < texture2d->GetSizeX(); x++)
//			{
//				int i = x + (y * texture2d->GetSizeX());
//				frame.data[i * 3 + 0]= FormatedImageData2[i].B;
//				frame.data[i * 3 + 1]= FormatedImageData2[i].G;
//				frame.data[i * 3 + 2]= FormatedImageData2[i].R;
//			}
//		}
//
//	for (int y = 0; y < viewportY; y++)
//	{
//		for (int x = 0; x < viewportX; x++)
//		{
//			int i = x + (y * viewportX);
//			frame.data[i * 3 + 0] = ViewportImage[i].B;
//			frame.data[i * 3 + 1] = ViewportImage[i].G;
//			frame.data[i * 3 + 2] = ViewportImage[i].R;
//		}
//	}
//
//	cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
//	dstImage = cv::Mat::zeros(frame.size(), CV_32FC1);
//	cornerHarris(gray, dstImage, 2, 3, 0.04);  //进行角点检测
//	normalize(dstImage, normImage, 0, 255, cv::NORM_MINMAX);  //归一化
//	convertScaleAbs(normImage, scaledImage);  //转换为8位无符号整形
//											  //将检测到的，且符合阈值条件的角点绘制出来
//	for (int j = 0; j < normImage.rows; j++)
//	{
//		for (int i = 0; i < normImage.cols; i++) 
//		{
//			if ((int)normImage.at<float>(j, i)>threshold) 
//			{
//				cv::circle(frame, cv::Point(i, j), 5, cv::Scalar(10, 10, 255), 2);
//				sum++;
//				if (sum >= 5)
//					goto flag;
//
//			}
//		}
//	}
//	
//flag:
//	sum = 0;
//	cv::imshow("1234", frame);
	

}

//UTexture2D* AcvDepthEstimator::LoadTexture2D(const FString& ImagePath, bool& IsValid, int32& OutWidth, int32& OutHeight)
//{
//	UTexture2D* Texture = nullptr;
//	IsValid = false;
//	if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*ImagePath))
//	{
//		return nullptr;
//	}
//	TArray<uint8> CompressedData;
//	if (!FFileHelper::LoadFileToArray(CompressedData, *ImagePath))
//	{
//		return nullptr;
//	}
//	TSharedPtr<IImageWrapper> ImageWrapper = GetImageWrapperByExtention(ImagePath);
//	if (ImageWrapper.IsValid() && ImageWrapper->SetCompressed(CompressedData.GetData(), CompressedData.Num()))
//	{
//		const TArray<uint8>* UncompressedRGBA = nullptr;
//		if (ImageWrapper->GetRaw(ERGBFormat::RGBA, 8, UncompressedRGBA))
//		{
//			Texture = UTexture2D::CreateTransient(ImageWrapper->GetWidth(), ImageWrapper->GetHeight(), PF_R8G8B8A8);
//			if (Texture != nullptr)
//			{
//				IsValid = true;
//				OutWidth = ImageWrapper->GetWidth();
//				OutHeight = ImageWrapper->GetHeight();
//				void* TextureData = Texture->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
//				FMemory::Memcpy(TextureData, UncompressedRGBA->GetData(), UncompressedRGBA->Num());
//				Texture->PlatformData->Mips[0].BulkData.Unlock();
//				Texture->UpdateResource();
//			}
//		}
//	}
//	return Texture;
//}
//
//UTexture2D* AcvDepthEstimator::GetTexture2DFromDiskFile(const FString& FilePath, bool& isvalid , int&a, int& b)
//{
//	isvalid = false;
//	TArray<uint8> RawFileData;
//	UTexture2D* MyTexture = NULL;
//	if (FFileHelper::LoadFileToArray(RawFileData, *FilePath /*"<path to file>"*/))
//	{
//		IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
//		// Note: PNG format.  Other formats are supported
//		TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::JPEG);
//		if (ImageWrapper.IsValid() && ImageWrapper->SetCompressed(RawFileData.GetData(), RawFileData.Num()))
//		{
//			const TArray<uint8>* UncompressedBGRA = NULL;
//			if (ImageWrapper->GetRaw(ERGBFormat::BGRA, 8, UncompressedBGRA))
//			{
//				// Create the UTexture for rendering
//				MyTexture = UTexture2D::CreateTransient(ImageWrapper->GetWidth(), ImageWrapper->GetHeight(), PF_B8G8R8A8);
//				a = ImageWrapper->GetWidth();
//				b = ImageWrapper->GetHeight();
//				// Fill in the source data from the file
//				void* TextureData = MyTexture->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
//				FMemory::Memcpy(TextureData, UncompressedBGRA->GetData(), UncompressedBGRA->Num());
//				MyTexture->PlatformData->Mips[0].BulkData.Unlock();
//				isvalid = true;
//				// Update the rendering resource from data.
//				MyTexture->UpdateResource();
//			}
//		}
//	}
//	return MyTexture;
//}
//
//TSharedPtr<IImageWrapper> AcvDepthEstimator::GetImageWrapperByExtention(const FString InImagePath)
//{
//	IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
//	if (InImagePath.EndsWith(".png"))
//	{
//		return ImageWrapperModule.CreateImageWrapper(EImageFormat::PNG);
//	}
//	else if (InImagePath.EndsWith(".jpg") || InImagePath.EndsWith(".jpeg"))
//	{
//		return ImageWrapperModule.CreateImageWrapper(EImageFormat::JPEG);
//	}
//	else if (InImagePath.EndsWith(".bmp"))
//	{
//		return ImageWrapperModule.CreateImageWrapper(EImageFormat::BMP);
//	}
//	else if (InImagePath.EndsWith(".ico"))
//	{
//		return ImageWrapperModule.CreateImageWrapper(EImageFormat::ICO);
//	}
//	else if (InImagePath.EndsWith(".exr"))
//	{
//		return ImageWrapperModule.CreateImageWrapper(EImageFormat::EXR);
//	}
//	else if (InImagePath.EndsWith(".icns"))
//	{
//		return ImageWrapperModule.CreateImageWrapper(EImageFormat::ICNS);
//	}
//	return nullptr;
//}
