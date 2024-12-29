//Copyright 2022~2024 DevGrain. All Rights Reserved.

using UnrealBuildTool;

public class VoltEditor : ModuleRules
{
	public VoltEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		PrecompileForTargets = PrecompileTargetsType.Any;

		//The path for the source files
		PrivateIncludePaths.AddRange(new[]
		{
			"VoltEditor",

			"VoltEditor/Public",
			
			"VoltEditor/Public/Editor",
			"VoltEditor/Public/Editor/Setting",
			"VoltEditor/Public/Editor/Slate",
			"VoltEditor/Public/Editor/Style",
			"VoltEditor/Public/Editor/Toolkit",
		});


		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"UnrealEd",
			
			"Slate",
			"SlateCore",
			"RenderCore",
			"UMG",
			"DesktopWidgets",
			
			"VoltCore",
			"Volt",
			
			"EditorStyle",
			"EditorWidgets",

			"PropertyEditor",
			"ToolMenus",
			
			"ContentBrowser",
			"DeveloperSettings",
			
			"InputCore", 
			"WorkspaceMenuStructure",
			"SettingsEditor",
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
			"EditorWidgets"
		});
	}
}