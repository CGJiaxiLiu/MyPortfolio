// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Dxerr.h"

#define LOG(format, ...) UE_LOG(LogTemp, Warning, TEXT(format), ##__VA_ARGS__)

#define RETURN_FALSE_IF_ERROR(result, content) \
{\
	if (FAILED(result))\
	{\
		FString content = FString(#content);\
		FString format =  FString(" Fail:");\
		FString describ = FString(DXGetErrorDescription(result));\
		UE_LOG(LogTemp, Warning, TEXT("%s"), *(content + format + describ));\
		return false;\
	}\
}

#define INIT_MEMORY(var) ZeroMemory(&var, sizeof(var));

class DXManager * GetDXManager();
