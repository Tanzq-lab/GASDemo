// Copyright Amos Tan

using UnrealBuildTool;
using System.Collections.Generic;

public class GASDemoTarget : TargetRules
{
	public GASDemoTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V4;

		ExtraModuleNames.AddRange( new string[] { "GASDemo" } );
	}
}
