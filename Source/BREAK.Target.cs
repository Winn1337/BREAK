// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class BREAKTarget : TargetRules
{
	public BREAKTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_5;
		ExtraModuleNames.Add("BREAK");
		ExtraModuleNames.Add("GrapplingHook");
        ExtraModuleNames.Add("BREAKRocketLauncher");
    }
}
