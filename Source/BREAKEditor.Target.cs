// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class BREAKEditorTarget : TargetRules
{
	public BREAKEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_5;
		ExtraModuleNames.Add("BREAK");
		ExtraModuleNames.Add("GrapplingHook");
        ExtraModuleNames.Add("BREAKRocketLauncher");
    }
}
