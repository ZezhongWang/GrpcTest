// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GrpcTest : ModuleRules
{
	public GrpcTest(ReadOnlyTargetRules Target) : base(Target)
	{
		PrivateDependencyModuleNames.AddRange(new string[] { "TurboLinkGrpc" });
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput" });
	}
}
