// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class ChronoPhysics : ModuleRules
{
	public ChronoPhysics(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core" });
        PrivateDependencyModuleNames.AddRange(new string[] { "CoreUObject", "Engine", "Slate", "SlateCore", "Projects", "RHI", "RenderCore", "ProceduralMeshComponent", "Json", "JsonUtilities" });

        PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Include"));
        PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Include", "chrono"));
        PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Include", "blaze-2.2"));
        PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Include", "boost_1_63_0"));
        PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Include", "chrono_parallel"));
        PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Include", "chrono", "collision", "bullet"));
        PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Include", "chrono", "collision", "gimpact"));
        PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Include", "chrono", "collision", "convexdecomposition", "HACD"));
        PublicLibraryPaths.Add(Path.Combine(ModuleDirectory, "Lib"));
        PublicAdditionalLibraries.Add("ChronoEngine.lib");
        PublicAdditionalLibraries.Add("ChronoEngine_vehicle.lib");
        PublicAdditionalLibraries.Add("ChronoEngine_parallel.lib");
        PublicAdditionalLibraries.Add("vcomp.lib");

        bUseRTTI = true;
        bEnableExceptions = true;
        PrivateDefinitions.Add("DEBUG_LOG");
        PrivateDefinitions.Add("NOMINMAX");
    }
}
