// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class BREAKRocketLauncher : ModuleRules
{
	public BREAKRocketLauncher(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PrivateDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine"});
	}
}
