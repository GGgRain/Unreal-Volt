//Copyright 2022~2024 DevGrain. All Rights Reserved.

using UnrealBuildTool;

public class VoltEditor : ModuleRules
{
	public VoltEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		PrecompileForTargets = PrecompileTargetsType.Any;

		PublicDependencyModuleNames.AddRange(new string[] { "Volt" });

		//The path for the source files

		PrivateIncludePaths.AddRange(new[]
		{
			"VoltEditor/Public",
			"VoltEditor/Public/EditorSlate",
		});


		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"UnrealEd",
			"ApplicationCore",
			"Slate",
			"SlateCore",
			"VoltCore",
			"Volt",


			"Projects",
			"EditorStyle",
		});

		if (Target.Version.MajorVersion >= 5)
		{
			PrivateDependencyModuleNames.Add("ToolWidgets");
		}

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"AssetTools",
			"AssetRegistry",
			"ClassViewer",
		});
	}
}