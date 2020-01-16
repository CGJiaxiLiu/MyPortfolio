// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class opencvLib : ModuleRules
{
	public opencvLib(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "RHI", "Engine", "ImageWrapper", "InputCore", "Projects"});

        PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Include"));

        PublicLibraryPaths.Add(Path.Combine(ModuleDirectory, "Lib"));
        PublicAdditionalLibraries.Add("opencv_world320.lib");
    }
}
