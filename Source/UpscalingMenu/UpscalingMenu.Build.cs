// ZumoDePapaya 2025 All Rights Reserved

using UnrealBuildTool;

public class UpscalingMenu : ModuleRules
{
	public UpscalingMenu(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
			}
		);

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"RenderCore",
				"RHI",
				"Projects",
				"Json",
				"JsonUtilities"
			}
		);

		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			PublicAdditionalLibraries.Add("Advapi32.lib");
		}
	}
}