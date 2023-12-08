// Copyright Amos Tan

using UnrealBuildTool;
using System.Collections.Generic;

public class GASDemoEditorTarget : TargetRules
{
	public GASDemoEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V4;

		ExtraModuleNames.AddRange( new string[] { "GASDemo" } );
	}
}
