// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.IO;

public class DXRender : ModuleRules
{
	public DXRender(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
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

        //PrivateDefinitions.Add("RENDER_IN_WINDOW");

    }
}
