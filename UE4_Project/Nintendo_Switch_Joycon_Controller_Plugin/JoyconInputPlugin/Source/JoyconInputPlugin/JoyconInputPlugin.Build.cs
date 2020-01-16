// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class JoyconInputPlugin : ModuleRules
{
	public JoyconInputPlugin(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
        PublicDependencyModuleNames.AddRange(new string[]{ "Core", "CoreUObject", "Engine", "InputCore", "Slate", "SlateCore", "ApplicationCore", "Projects" });
        PrivateDependencyModuleNames.AddRange(new string[] { "InputDevice" });

        PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Include"));
        PublicLibraryPaths.Add(Path.Combine(ModuleDirectory, "Bin"));
        PublicLibraryPaths.Add(Path.Combine(ModuleDirectory, "Lib"));

        PublicAdditionalLibraries.Add("JoyconProject.lib");
        PublicDelayLoadDLLs.Add("JoyconProject.dll");
        PublicDelayLoadDLLs.Add("hidapi.dll");

    }
}
