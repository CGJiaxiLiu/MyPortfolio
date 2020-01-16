// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "opencvLib/Public/WebcamReader.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeWebcamReader() {}
// Cross Module References
	OPENCVLIB_API UClass* Z_Construct_UClass_AWebcamReader_NoRegister();
	OPENCVLIB_API UClass* Z_Construct_UClass_AWebcamReader();
	ENGINE_API UClass* Z_Construct_UClass_AActor();
	UPackage* Z_Construct_UPackage__Script_opencvLib();
	ENGINE_API UClass* Z_Construct_UClass_UTextureRenderTarget2D_NoRegister();
// End Cross Module References
	void AWebcamReader::StaticRegisterNativesAWebcamReader()
	{
	}
	UClass* Z_Construct_UClass_AWebcamReader_NoRegister()
	{
		return AWebcamReader::StaticClass();
	}
	struct Z_Construct_UClass_AWebcamReader_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_SceneCapture_3_MetaData[];
#endif
		static const UE4CodeGen_Private::FObjectPropertyParams NewProp_SceneCapture_3;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_SceneCapture_2_MetaData[];
#endif
		static const UE4CodeGen_Private::FObjectPropertyParams NewProp_SceneCapture_2;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_SceneCapture_1_MetaData[];
#endif
		static const UE4CodeGen_Private::FObjectPropertyParams NewProp_SceneCapture_1;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_maxpoint_MetaData[];
#endif
		static const UE4CodeGen_Private::FUnsizedIntPropertyParams NewProp_maxpoint;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_threshold_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_threshold;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_AWebcamReader_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_AActor,
		(UObject* (*)())Z_Construct_UPackage__Script_opencvLib,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AWebcamReader_Statics::Class_MetaDataParams[] = {
		{ "IncludePath", "WebcamReader.h" },
		{ "ModuleRelativePath", "Public/WebcamReader.h" },
	};
#endif
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AWebcamReader_Statics::NewProp_SceneCapture_3_MetaData[] = {
		{ "Category", "Webcam" },
		{ "ModuleRelativePath", "Public/WebcamReader.h" },
	};
#endif
	const UE4CodeGen_Private::FObjectPropertyParams Z_Construct_UClass_AWebcamReader_Statics::NewProp_SceneCapture_3 = { "SceneCapture_3", nullptr, (EPropertyFlags)0x0010000000000001, UE4CodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(AWebcamReader, SceneCapture_3), Z_Construct_UClass_UTextureRenderTarget2D_NoRegister, METADATA_PARAMS(Z_Construct_UClass_AWebcamReader_Statics::NewProp_SceneCapture_3_MetaData, ARRAY_COUNT(Z_Construct_UClass_AWebcamReader_Statics::NewProp_SceneCapture_3_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AWebcamReader_Statics::NewProp_SceneCapture_2_MetaData[] = {
		{ "Category", "Webcam" },
		{ "ModuleRelativePath", "Public/WebcamReader.h" },
	};
#endif
	const UE4CodeGen_Private::FObjectPropertyParams Z_Construct_UClass_AWebcamReader_Statics::NewProp_SceneCapture_2 = { "SceneCapture_2", nullptr, (EPropertyFlags)0x0010000000000001, UE4CodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(AWebcamReader, SceneCapture_2), Z_Construct_UClass_UTextureRenderTarget2D_NoRegister, METADATA_PARAMS(Z_Construct_UClass_AWebcamReader_Statics::NewProp_SceneCapture_2_MetaData, ARRAY_COUNT(Z_Construct_UClass_AWebcamReader_Statics::NewProp_SceneCapture_2_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AWebcamReader_Statics::NewProp_SceneCapture_1_MetaData[] = {
		{ "Category", "Webcam" },
		{ "ModuleRelativePath", "Public/WebcamReader.h" },
	};
#endif
	const UE4CodeGen_Private::FObjectPropertyParams Z_Construct_UClass_AWebcamReader_Statics::NewProp_SceneCapture_1 = { "SceneCapture_1", nullptr, (EPropertyFlags)0x0010000000000001, UE4CodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(AWebcamReader, SceneCapture_1), Z_Construct_UClass_UTextureRenderTarget2D_NoRegister, METADATA_PARAMS(Z_Construct_UClass_AWebcamReader_Statics::NewProp_SceneCapture_1_MetaData, ARRAY_COUNT(Z_Construct_UClass_AWebcamReader_Statics::NewProp_SceneCapture_1_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AWebcamReader_Statics::NewProp_maxpoint_MetaData[] = {
		{ "Category", "WebcamReader" },
		{ "ModuleRelativePath", "Public/WebcamReader.h" },
	};
#endif
	const UE4CodeGen_Private::FUnsizedIntPropertyParams Z_Construct_UClass_AWebcamReader_Statics::NewProp_maxpoint = { "maxpoint", nullptr, (EPropertyFlags)0x0010000000000005, UE4CodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(AWebcamReader, maxpoint), METADATA_PARAMS(Z_Construct_UClass_AWebcamReader_Statics::NewProp_maxpoint_MetaData, ARRAY_COUNT(Z_Construct_UClass_AWebcamReader_Statics::NewProp_maxpoint_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AWebcamReader_Statics::NewProp_threshold_MetaData[] = {
		{ "Category", "WebcamReader" },
		{ "ModuleRelativePath", "Public/WebcamReader.h" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_AWebcamReader_Statics::NewProp_threshold = { "threshold", nullptr, (EPropertyFlags)0x0010000000000005, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(AWebcamReader, threshold), METADATA_PARAMS(Z_Construct_UClass_AWebcamReader_Statics::NewProp_threshold_MetaData, ARRAY_COUNT(Z_Construct_UClass_AWebcamReader_Statics::NewProp_threshold_MetaData)) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_AWebcamReader_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AWebcamReader_Statics::NewProp_SceneCapture_3,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AWebcamReader_Statics::NewProp_SceneCapture_2,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AWebcamReader_Statics::NewProp_SceneCapture_1,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AWebcamReader_Statics::NewProp_maxpoint,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AWebcamReader_Statics::NewProp_threshold,
	};
	const FCppClassTypeInfoStatic Z_Construct_UClass_AWebcamReader_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<AWebcamReader>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_AWebcamReader_Statics::ClassParams = {
		&AWebcamReader::StaticClass,
		nullptr,
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		Z_Construct_UClass_AWebcamReader_Statics::PropPointers,
		nullptr,
		ARRAY_COUNT(DependentSingletons),
		0,
		ARRAY_COUNT(Z_Construct_UClass_AWebcamReader_Statics::PropPointers),
		0,
		0x009000A0u,
		METADATA_PARAMS(Z_Construct_UClass_AWebcamReader_Statics::Class_MetaDataParams, ARRAY_COUNT(Z_Construct_UClass_AWebcamReader_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_AWebcamReader()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_AWebcamReader_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(AWebcamReader, 2458385647);
	template<> OPENCVLIB_API UClass* StaticClass<AWebcamReader>()
	{
		return AWebcamReader::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_AWebcamReader(Z_Construct_UClass_AWebcamReader, &AWebcamReader::StaticClass, TEXT("/Script/opencvLib"), TEXT("AWebcamReader"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(AWebcamReader);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
