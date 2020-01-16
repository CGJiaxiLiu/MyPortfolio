// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class DXRenderPlugin : ModuleRules
{
	public DXRenderPlugin(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });

        PrivateDependencyModuleNames.AddRange(new string[] { "RHI", "RenderCore", "MeshDescription", "RawMesh" });

        PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Include"));
        PublicLibraryPaths.Add(Path.Combine(ModuleDirectory, "Lib"));
        PublicAdditionalLibraries.Add("dxgi.lib");
        PublicAdditionalLibraries.Add("d3d11.lib");
        PublicAdditionalLibraries.Add("d3dx11.lib");
        PublicAdditionalLibraries.Add("d3dx10.lib");
        PublicAdditionalLibraries.Add("Dxerr.lib");
        PublicAdditionalLibraries.Add("legacy_stdio_definitions.lib");
    }
}
