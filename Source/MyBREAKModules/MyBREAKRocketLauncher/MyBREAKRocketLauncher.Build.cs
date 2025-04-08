// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MyBREAKRocketLauncher : ModuleRules
{
	public MyBREAKRocketLauncher(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PrivateDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine"});
	}
}
