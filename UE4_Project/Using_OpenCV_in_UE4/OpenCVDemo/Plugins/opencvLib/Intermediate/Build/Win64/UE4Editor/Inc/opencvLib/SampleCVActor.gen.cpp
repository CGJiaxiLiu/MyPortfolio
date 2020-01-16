// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "opencvLib/Public/SampleCVActor.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeSampleCVActor() {}
// Cross Module References
	OPENCVLIB_API UClass* Z_Construct_UClass_ASampleCVActor_NoRegister();
	OPENCVLIB_API UClass* Z_Construct_UClass_ASampleCVActor();
	ENGINE_API UClass* Z_Construct_UClass_AActor();
	UPackage* Z_Construct_UPackage__Script_opencvLib();
// End Cross Module References
	void ASampleCVActor::StaticRegisterNativesASampleCVActor()
	{
	}
	UClass* Z_Construct_UClass_ASampleCVActor_NoRegister()
	{
		return ASampleCVActor::StaticClass();
	}
	struct Z_Construct_UClass_ASampleCVActor_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_ASampleCVActor_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_AActor,
		(UObject* (*)())Z_Construct_UPackage__Script_opencvLib,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ASampleCVActor_Statics::Class_MetaDataParams[] = {
		{ "IncludePath", "SampleCVActor.h" },
		{ "ModuleRelativePath", "Public/SampleCVActor.h" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_ASampleCVActor_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<ASampleCVActor>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_ASampleCVActor_Statics::ClassParams = {
		&ASampleCVActor::StaticClass,
		nullptr,
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		nullptr,
		nullptr,
		ARRAY_COUNT(DependentSingletons),
		0,
		0,
		0,
		0x009000A0u,
		METADATA_PARAMS(Z_Construct_UClass_ASampleCVActor_Statics::Class_MetaDataParams, ARRAY_COUNT(Z_Construct_UClass_ASampleCVActor_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_ASampleCVActor()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_ASampleCVActor_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(ASampleCVActor, 2955862702);
	template<> OPENCVLIB_API UClass* StaticClass<ASampleCVActor>()
	{
		return ASampleCVActor::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_ASampleCVActor(Z_Construct_UClass_ASampleCVActor, &ASampleCVActor::StaticClass, TEXT("/Script/opencvLib"), TEXT("ASampleCVActor"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(ASampleCVActor);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
