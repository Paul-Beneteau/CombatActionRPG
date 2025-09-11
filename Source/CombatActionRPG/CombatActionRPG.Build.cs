// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CombatActionRPG : ModuleRules
{
	public CombatActionRPG(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "NavigationSystem", "AIModule", "Niagara" });
	}
}
