//Copyright 2022~2024 DevGrain. All Rights Reserved.

#include "VoltEditor.h"

#include "VoltEditorCommands.h"

#include "VoltEditorStyle.h"
#include "WorkspaceMenuStructure.h"
#include "WorkspaceMenuStructureModule.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "Starship/VoltStarship.h"
#include "Widgets/Docking/SDockTab.h"


#define LOCTEXT_NAMESPACE "VoltEditorModule"


void FVoltEditorModule::StartupModule()
{

	FVoltEditorStyle::ResetToDefault();
	
	RegisterModuleCommand();
	
	RegisterMenuExtensions();

	RegisterMajorTabs();

}


void FVoltEditorModule::RegisterMajorTabs()
{
	FTabSpawnerEntry& TabSpawnerEntry = FGlobalTabmanager::Get()->RegisterNomadTabSpawner(
			VoltToolTabNames::VoltStarshipTab,
			FOnSpawnTab::CreateRaw(this, &FVoltEditorModule::OnSpawnVoltStarshipTab))
		.SetDisplayName(LOCTEXT("VoltStarshipDisplayName", "Volt Starship"))
		.SetMenuType(ETabSpawnerMenuType::Enabled)
		.SetGroup(WorkspaceMenu::GetMenuStructure().GetToolsCategory())
		.SetIcon(FSlateIcon(FVoltEditorStyle::GetUEEditorSlateStyleSetName(), "Icons.Star"));
}


void FVoltEditorModule::UnregisterMajorTabs()
{
	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(VoltToolTabNames::VoltStarshipTab);
}

void FVoltEditorModule::ShutdownModule()
{
	
	UnregisterMenuExtensions();
	
	UnregisterMajorTabs();

}

bool FVoltEditorModule::SupportsDynamicReloading()
{
	return true;
}

void FVoltEditorModule::RegisterModuleCommand()
{
	FVoltEditorCommands::Register();

	FVoltEditorCommands::Get().PluginCommands->MapAction(
		FVoltEditorCommands::Get().OpenVoltStarshipTab
		, FExecuteAction::CreateRaw(this, &FVoltEditorModule::OpenVoltStarshipTab)
	);
}

void FVoltEditorModule::OpenVoltStarshipTab()
{
	FGlobalTabmanager::Get()->TryInvokeTab(VoltToolTabNames::VoltStarshipTab);

}

TSharedRef<SDockTab> FVoltEditorModule::OnSpawnVoltStarshipTab(const FSpawnTabArgs& SpawnTabArgs)
{
	auto NomadTab = SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		.Label(LOCTEXT("VoltStarshipTabTitle", "Volt Starship"));

	auto TabManager = FGlobalTabmanager::Get()->NewTabManager(NomadTab);
	TabManager->SetOnPersistLayout(
		FTabManager::FOnPersistLayout::CreateStatic(
			[](const TSharedRef<FTabManager::FLayout>& InLayout)
			{
				if (InLayout->GetPrimaryArea().Pin().IsValid())
				{
					FLayoutSaveRestore::SaveToConfig(GEditorLayoutIni, InLayout);
				}
			}
		)
	);

	NomadTab->SetTabIcon(FVoltEditorStyle::GetUEEditorSlateStyleSet().GetBrush("Icons.Star")); 

	NomadTab->SetOnTabClosed(
		SDockTab::FOnTabClosedCallback::CreateStatic(
			[](TSharedRef<SDockTab> Self, TWeakPtr<FTabManager> TabManager)
			{
				TSharedPtr<FTabManager> OwningTabManager = TabManager.Pin();
				if (OwningTabManager.IsValid())
				{
					FLayoutSaveRestore::SaveToConfig(GEditorLayoutIni, OwningTabManager->PersistLayout());
					OwningTabManager->CloseAllAreas();
				}
			}
			, TWeakPtr<FTabManager>(TabManager)
		)
	);

	FVoltEditorModule& EditorModule = FModuleManager::GetModuleChecked<FVoltEditorModule>("VoltEditor");

	auto MainWidget = SNew(SVoltStarship)
		.TabManager(TabManager);

	NomadTab->SetContent(MainWidget);
	return NomadTab;
}

void FVoltEditorModule::RegisterMenuExtensions()
{
	MenuExtensibilityManager = MakeShareable(new FExtensibilityManager);
	ToolBarExtensibilityManager = MakeShareable(new FExtensibilityManager);
}

void FVoltEditorModule::UnregisterMenuExtensions()
{
	MenuExtensibilityManager.Reset();
	ToolBarExtensibilityManager.Reset();
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FVoltEditorModule, JointEditor)
