// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "opencvLib.h"
#include "Core.h"
#include "Modules/ModuleManager.h"
#include "Interfaces/IPluginManager.h"

#define LOCTEXT_NAMESPACE "FopencvLibModule"

void FopencvLibModule::StartupModule()
{

}

void FopencvLibModule::ShutdownModule()
{
	//ExampleLibraryHandle = nullptr;
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FopencvLibModule, opencvLib)
