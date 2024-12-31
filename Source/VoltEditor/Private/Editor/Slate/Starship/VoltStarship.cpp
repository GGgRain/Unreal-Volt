#include <Starship/VoltStarship.h>

#include "VoltDecl.h"
#include "VoltEditorStyle.h"
#include "Brushes/SlateRoundedBoxBrush.h"
#include "Chaos/AABB.h"
#include "Chaos/AABB.h"
#include "Module/Volt_ASM_InterpBackgroundColor.h"
#include "Module/Volt_ASM_InterpColor.h"
#include "Module/Volt_ASM_InterpWidgetTransform.h"
#include "Module/Volt_ASM_Sequence.h"
#include "Module/Volt_ASM_SetWidgetTransformPivot.h"
#include "Starship/VoltStarshipWidgets.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Layout/SWrapBox.h"

#define LOCTEXT_NAMESPACE "VoltStarship"

#define VOLT_STARSHIP_BORDER_MARGIN 4
#define VOLT_STARSHIP_BUTTON_CONTENT_MARGIN 2
#define VOLT_STARSHIP_MARGIN_GAP 2

#define VOLT_STARSHIP_BORDER_BACKGROUND_COLOR FLinearColor(0.01,0.01,0.01)
#define VOLT_STARSHIP_BORDER_BACKGROUND_COLOR_BRIGHT FLinearColor(0.03,0.03,0.03)
#define VOLT_STARSHIP_BORDER_BACKGROUND_COLOR_BRIGHT_HOVER FLinearColor(0.30,0.3,0.3)


namespace VoltModuleStarshipTab
{
	static const FName TabName = FName(TEXT("Volt Module Starship"));

	static void RegisterTabSpawner(const TSharedPtr<FTabManager>& TabManager)
	{
		TabManager->RegisterTabSpawner(
				TabName
				, FOnSpawnTab::CreateLambda(
					[=](const FSpawnTabArgs&)
					{
						return SNew(SDockTab)
							.TabRole(ETabRole::PanelTab)
							.Label(LOCTEXT("VoltModuleStarshipTabName", "Volt Module Starship"))
							[
								SNew(SVoltModuleStarshipTab)
							];
					}
				)
			)
			.SetDisplayName(LOCTEXT("VoltModuleStarshipTabTitle", "Volt Module Starship"))
			.SetTooltipText(LOCTEXT("VoltModuleStarshipTabTooltip", "Open the Volt Module Starship Tab."))
			.SetIcon(FSlateIcon(FVoltEditorStyle::GetUEEditorSlateStyleSetName(),
			                    "ExternalImagePicker.GenerateImageButton"));
	}
}


namespace VoltSampleStarshipTab
{
	static const FName TabName = FName(TEXT("Volt Sample Starship"));

	static void RegisterTabSpawner(const TSharedPtr<FTabManager>& TabManager)
	{
		TabManager->RegisterTabSpawner(
				TabName
				, FOnSpawnTab::CreateLambda(
					[=](const FSpawnTabArgs&)
					{
						return SNew(SDockTab)
							.TabRole(ETabRole::PanelTab)
							.Label(LOCTEXT("VoltSampleStarshipTabName", "Volt Sample Starship"))
							[
								SNew(SVoltSampleStarshipTab)
							];
					}
				)
			)
			.SetDisplayName(LOCTEXT("VoltSampleStarshipTabTitle", "Volt Sample Starship"))
			.SetTooltipText(LOCTEXT("VoltSampleStarshipTabTooltip",
			                        "Open the Volt Sample Starship tap. Full of good and useful animation samples!"))
			.SetIcon(FSlateIcon(FVoltEditorStyle::GetUEEditorSlateStyleSetName(), "PlacementBrowser.Icons.Testing"));
	}
}


void SVoltStarship::Construct(const FArguments& InArgs)
{
	TabManager = InArgs._TabManager;

	VoltModuleStarshipTab::RegisterTabSpawner(TabManager);
	VoltSampleStarshipTab::RegisterTabSpawner(TabManager);

	TSharedPtr<FTabManager::FLayout> DebuggerLayout = FTabManager::NewLayout("VoltStarshipLayout_V1.0")
		->AddArea(
			FTabManager::NewPrimaryArea()
			->SetOrientation(Orient_Horizontal)
			->Split(
				FTabManager::NewSplitter()
				->SetOrientation(Orient_Horizontal)
				->SetSizeCoefficient(1.f)
				->Split(
					FTabManager::NewStack()
					->SetSizeCoefficient(0.3f)
					->SetHideTabWell(false)
					->AddTab(VoltModuleStarshipTab::TabName, ETabState::OpenedTab)
					->SetForegroundTab(VoltModuleStarshipTab::TabName)
				)
				->Split(
					FTabManager::NewStack()
					->SetSizeCoefficient(0.7f)
					->SetHideTabWell(false)
					->AddTab(VoltSampleStarshipTab::TabName, ETabState::OpenedTab)
					->SetForegroundTab(VoltSampleStarshipTab::TabName)
				)
			)
		);

	DebuggerLayout = FLayoutSaveRestore::LoadFromConfig(GEditorLayoutIni, DebuggerLayout.ToSharedRef());

	TSharedRef<SWidget> TabContents = TabManager->RestoreFrom(DebuggerLayout.ToSharedRef(), TSharedPtr<SWindow>()).
		ToSharedRef();

	// create & initialize main menu
	FMenuBarBuilder MenuBarBuilder = FMenuBarBuilder(TSharedPtr<FUICommandList>());

	MenuBarBuilder.AddPullDownMenu(
		LOCTEXT("WindowMenuLabel", "Window")
		, FText::GetEmpty()
		, FNewMenuDelegate::CreateSP(this, &SVoltStarship::FillWindowMenu)
		, "Window"
	);

	// Tell tab-manager about the multi-box for platforms with a global menu bar

#if UE_VERSION_OLDER_THAN(5, 0, 0)
	TabManager->SetMenuMultiBox(MenuBarBuilder.GetMultiBox());
#else
	TSharedRef<SWidget> MenuWidget = MenuBarBuilder.MakeWidget();
	MenuWidget->SetClipping(EWidgetClipping::ClipToBoundsWithoutIntersecting);

	TabManager->SetMenuMultiBox(MenuBarBuilder.GetMultiBox(), MenuWidget);
#endif

	ChildSlot
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			MenuBarBuilder.MakeWidget()
		]
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			MakeToolbar()
		]
		+ SVerticalBox::Slot()
		.Padding(2.0)
		[
			TabContents
		]
	];
}

void SVoltStarship::FillWindowMenu(FMenuBuilder& MenuBuilder)
{
	if (!TabManager.IsValid()) { return; }

#if !WITH_EDITOR
	FGlobalTabmanager::Get()->PopulateTabSpawnerMenu(MenuBuilder, WorkspaceMenu::GetMenuStructure().GetStructureRoot());
#endif //!WITH_EDITOR

	TabManager->PopulateLocalTabSpawnerMenu(MenuBuilder);
}

TSharedRef<SWidget> SVoltStarship::MakeToolbar()
{
	FToolBarBuilder ToolbarBuilder(MakeShareable(new FUICommandList), FMultiBoxCustomization::None);
	ToolbarBuilder.BeginSection("Main");

	ToolbarBuilder.EndSection();
	
	return ToolbarBuilder.MakeWidget();
}


void SVoltModuleStarshipTab::Construct(const FArguments& InArgs)
{
	VOLT_IMPLEMENT_MANAGER(&AnimationManager, SharedThis(this));

	ChildSlot
		.VAlign(VAlign_Fill)
		.HAlign(HAlign_Fill)
		[
			SNew(SScrollBox)
			.AllowOverscroll(EAllowOverscroll::Yes)
			.AnimateWheelScrolling(true)
			+ SScrollBox::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Center)
			.Padding(VOLT_STARSHIP_BORDER_MARGIN)
			[
				SNew(STextBlock)
				.Text(INVTEXT("Module - Interpolation: RateBased"))
			]
			+ SScrollBox::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Center)
			.Padding(VOLT_STARSHIP_BORDER_MARGIN)
			[
				CreateInterpSection_Rate()
			]
			+ SScrollBox::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Center)
			.Padding(VOLT_STARSHIP_BORDER_MARGIN)
			[
				SNew(STextBlock)
				.Text(INVTEXT("Module - Interpolation: AlphaBased"))
			]
			+ SScrollBox::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Center)
			.Padding(VOLT_STARSHIP_BORDER_MARGIN)
			[
				CreateInterpSection_Alpha()
			]
		];
}

TSharedRef<SWidget> SVoltModuleStarshipTab::CreateInterpSection_Rate()
{
	TSharedRef<SWrapBox> Box = SNew(SWrapBox)
		.UseAllottedSize(true)
		.Orientation(Orient_Horizontal)
		+ SWrapBox::Slot()
		.ForceNewLine(false)
		.Padding(VOLT_STARSHIP_BORDER_MARGIN)
		[
			SNew(SVoltInterpModuleBackground)
			.Title(INVTEXT("RateBased : Constant"))
			.AnimationManager(AnimationManager)
			.AnimationToPlay(
				VOLT_MAKE_ANIMATION(UVoltAnimation)
				(
					VOLT_MAKE_MODULE(UVolt_ASM_SetWidgetTransformPivot)
					.TargetWidgetTransformPivot(FVector2D(0.5f, 0.5f)),
					VOLT_MAKE_MODULE(UVolt_ASM_Sequence)
					.bShouldLoop(true)
					(
						VOLT_MAKE_MODULE(UVolt_ASM_InterpWidgetTransform)
						.InterpolationMode(EVoltInterpMode::RateBased)
						.bRateBasedUseConstant(true)
						.TargetWidgetTransform(FWidgetTransform(FVector2D(50, 0), FVector2D(1, 1), FVector2D(0, 0), 0))
						.RateBasedInterpSpeed(30),
						VOLT_MAKE_MODULE(UVolt_ASM_InterpWidgetTransform)
						.InterpolationMode(EVoltInterpMode::RateBased)
						.bRateBasedUseConstant(true)
						.TargetWidgetTransform(FWidgetTransform(FVector2D(-50, 0), FVector2D(1, 1), FVector2D(0, 0), 0))
						.RateBasedInterpSpeed(30)
					)
				)
			)
		]
		+ SWrapBox::Slot()
		.ForceNewLine(false)
		.Padding(VOLT_STARSHIP_BORDER_MARGIN)
		[
			SNew(SVoltInterpModuleBackground)
			.Title(INVTEXT("RateBased : Linear"))
			.AnimationManager(AnimationManager)
			.AnimationToPlay(
				VOLT_MAKE_ANIMATION(UVoltAnimation)
				(
					VOLT_MAKE_MODULE(UVolt_ASM_SetWidgetTransformPivot)
					.TargetWidgetTransformPivot(FVector2D(0.5f, 0.5f)),
					VOLT_MAKE_MODULE(UVolt_ASM_Sequence)
					.bShouldLoop(true)
					(
						VOLT_MAKE_MODULE(UVolt_ASM_InterpWidgetTransform)
						.InterpolationMode(EVoltInterpMode::RateBased)
						.TargetWidgetTransform(FWidgetTransform(FVector2D(50, 0), FVector2D(1, 1), FVector2D(0, 0), 0))
						.RateBasedInterpSpeed(15),
						VOLT_MAKE_MODULE(UVolt_ASM_InterpWidgetTransform)
						.InterpolationMode(EVoltInterpMode::RateBased)
						.TargetWidgetTransform(FWidgetTransform(FVector2D(-50, 0), FVector2D(1, 1), FVector2D(0, 0), 0))
						.RateBasedInterpSpeed(15)
					)
				)
			)
		];

	return Box;
}

TSharedRef<SWidget> SVoltModuleStarshipTab::CreateInterpSection_Alpha()
{
	TSharedRef<SWrapBox> Box = SNew(SWrapBox)
		.UseAllottedSize(true)
		.Orientation(Orient_Horizontal)
		+ SWrapBox::Slot()
		.ForceNewLine(false)
		.Padding(VOLT_STARSHIP_BORDER_MARGIN)
		[
			SNew(SVoltInterpModuleBackground)
			.Title(INVTEXT("AlphaBased : Linear"))
			.AnimationManager(AnimationManager)
			.AnimationToPlay(
				VOLT_MAKE_ANIMATION(UVoltAnimation)(
					VOLT_MAKE_MODULE(UVolt_ASM_SetWidgetTransformPivot)
					.TargetWidgetTransformPivot(FVector2D(0.5f, 0.5f)),
					VOLT_MAKE_MODULE(UVolt_ASM_Sequence)
					.bShouldLoop(true)
					(
						VOLT_MAKE_MODULE(UVolt_ASM_InterpWidgetTransform)
						.InterpolationMode(EVoltInterpMode::AlphaBased)
						.AlphaBasedEasingFunction(EEasingFunc::Linear)
						.bUseStartWidgetTransform(true)
						.StartWidgetTransform(FWidgetTransform(FVector2D(-50, 0), FVector2D(1, 1), FVector2D(0, 0), 0))
						.TargetWidgetTransform(FWidgetTransform(FVector2D(50, 0), FVector2D(1, 1), FVector2D(0, 0), 0))
						.AlphaBasedDuration(3),
						VOLT_MAKE_MODULE(UVolt_ASM_InterpWidgetTransform)
						.InterpolationMode(EVoltInterpMode::AlphaBased)
						.AlphaBasedEasingFunction(EEasingFunc::Linear)
						.bUseStartWidgetTransform(true)
						.StartWidgetTransform(FWidgetTransform(FVector2D(50, 0), FVector2D(1, 1), FVector2D(0, 0), 0))
						.TargetWidgetTransform(FWidgetTransform(FVector2D(-50, 0), FVector2D(1, 1), FVector2D(0, 0), 0))
						.AlphaBasedDuration(3)
					)
				)
			)
		]
		+ SWrapBox::Slot()
		.ForceNewLine(false)
		.Padding(VOLT_STARSHIP_BORDER_MARGIN)
		[
			SNew(SVoltInterpModuleBackground)
			.Title(INVTEXT("AlphaBased : Step"))
			.AnimationManager(AnimationManager)
			.AnimationToPlay(
				VOLT_MAKE_ANIMATION(UVoltAnimation)(
					VOLT_MAKE_MODULE(UVolt_ASM_SetWidgetTransformPivot)
					.TargetWidgetTransformPivot(FVector2D(0.5f, 0.5f)),
					VOLT_MAKE_MODULE(UVolt_ASM_Sequence)
					.bShouldLoop(true)
					(
						VOLT_MAKE_MODULE(UVolt_ASM_InterpWidgetTransform)
						.InterpolationMode(EVoltInterpMode::AlphaBased)
						.AlphaBasedEasingFunction(EEasingFunc::Step)
						.bUseStartWidgetTransform(true)
						.StartWidgetTransform(FWidgetTransform(FVector2D(-50, 0), FVector2D(1, 1), FVector2D(0, 0), 0))
						.TargetWidgetTransform(FWidgetTransform(FVector2D(50, 0), FVector2D(1, 1), FVector2D(0, 0), 0))
						.AlphaBasedSteps(5)
						.AlphaBasedDuration(3),
						VOLT_MAKE_MODULE(UVolt_ASM_InterpWidgetTransform)
						.InterpolationMode(EVoltInterpMode::AlphaBased)
						.AlphaBasedEasingFunction(EEasingFunc::Step)
						.bUseStartWidgetTransform(true)
						.StartWidgetTransform(FWidgetTransform(FVector2D(50, 0), FVector2D(1, 1), FVector2D(0, 0), 0))
						.TargetWidgetTransform(FWidgetTransform(FVector2D(-50, 0), FVector2D(1, 1), FVector2D(0, 0), 0))
						.AlphaBasedSteps(5)
						.AlphaBasedDuration(3)
					)
				)
			)
		]
		+ SWrapBox::Slot()
		.ForceNewLine(false)
		.Padding(VOLT_STARSHIP_BORDER_MARGIN)
		[
			SNew(SVoltInterpModuleBackground)
			.Title(INVTEXT("AlphaBased : CircularIn"))
			.AnimationManager(AnimationManager)
			.AnimationToPlay(
				VOLT_MAKE_ANIMATION(UVoltAnimation)(
					VOLT_MAKE_MODULE(UVolt_ASM_SetWidgetTransformPivot)
					.TargetWidgetTransformPivot(FVector2D(0.5f, 0.5f)),
					VOLT_MAKE_MODULE(UVolt_ASM_Sequence)
					.bShouldLoop(true)
					(
						VOLT_MAKE_MODULE(UVolt_ASM_InterpWidgetTransform)
						.InterpolationMode(EVoltInterpMode::AlphaBased)
						.AlphaBasedEasingFunction(EEasingFunc::CircularIn)
						.bUseStartWidgetTransform(true)
						.StartWidgetTransform(FWidgetTransform(FVector2D(-50, 0), FVector2D(1, 1), FVector2D(0, 0), 0))
						.TargetWidgetTransform(FWidgetTransform(FVector2D(50, 0), FVector2D(1, 1), FVector2D(0, 0), 0))
						.AlphaBasedBlendExp(2)
						.AlphaBasedDuration(3),
						VOLT_MAKE_MODULE(UVolt_ASM_InterpWidgetTransform)
						.InterpolationMode(EVoltInterpMode::AlphaBased)
						.AlphaBasedEasingFunction(EEasingFunc::CircularIn)
						.bUseStartWidgetTransform(true)
						.StartWidgetTransform(FWidgetTransform(FVector2D(50, 0), FVector2D(1, 1), FVector2D(0, 0), 0))
						.TargetWidgetTransform(FWidgetTransform(FVector2D(-50, 0), FVector2D(1, 1), FVector2D(0, 0), 0))
						.AlphaBasedBlendExp(2)
						.AlphaBasedDuration(3)
					)
				)
			)
		]
		+ SWrapBox::Slot()
		.ForceNewLine(false)
		.Padding(VOLT_STARSHIP_BORDER_MARGIN)
		[
			SNew(SVoltInterpModuleBackground)
			.Title(INVTEXT("AlphaBased : CircularOut"))
			.AnimationManager(AnimationManager)
			.AnimationToPlay(
				VOLT_MAKE_ANIMATION(UVoltAnimation)(
					VOLT_MAKE_MODULE(UVolt_ASM_SetWidgetTransformPivot)
					.TargetWidgetTransformPivot(FVector2D(0.5f, 0.5f)),
					VOLT_MAKE_MODULE(UVolt_ASM_Sequence)
					.bShouldLoop(true)
					(
						VOLT_MAKE_MODULE(UVolt_ASM_InterpWidgetTransform)
						.InterpolationMode(EVoltInterpMode::AlphaBased)
						.AlphaBasedEasingFunction(EEasingFunc::CircularOut)
						.bUseStartWidgetTransform(true)
						.StartWidgetTransform(FWidgetTransform(FVector2D(-50, 0), FVector2D(1, 1), FVector2D(0, 0), 0))
						.TargetWidgetTransform(FWidgetTransform(FVector2D(50, 0), FVector2D(1, 1), FVector2D(0, 0), 0))
						.AlphaBasedBlendExp(2)
						.AlphaBasedDuration(3),
						VOLT_MAKE_MODULE(UVolt_ASM_InterpWidgetTransform)
						.InterpolationMode(EVoltInterpMode::AlphaBased)
						.AlphaBasedEasingFunction(EEasingFunc::CircularOut)
						.bUseStartWidgetTransform(true)
						.StartWidgetTransform(FWidgetTransform(FVector2D(50, 0), FVector2D(1, 1), FVector2D(0, 0), 0))
						.TargetWidgetTransform(FWidgetTransform(FVector2D(-50, 0), FVector2D(1, 1), FVector2D(0, 0), 0))
						.AlphaBasedBlendExp(2)
						.AlphaBasedDuration(3)
					)
				)
			)
		]
		+ SWrapBox::Slot()
		.ForceNewLine(false)
		.Padding(VOLT_STARSHIP_BORDER_MARGIN)
		[
			SNew(SVoltInterpModuleBackground)
			.Title(INVTEXT("AlphaBased : CircularInOut"))
			.AnimationManager(AnimationManager)
			.AnimationToPlay(
				VOLT_MAKE_ANIMATION(UVoltAnimation)(
					VOLT_MAKE_MODULE(UVolt_ASM_SetWidgetTransformPivot)
					.TargetWidgetTransformPivot(FVector2D(0.5f, 0.5f)),
					VOLT_MAKE_MODULE(UVolt_ASM_Sequence)
					.bShouldLoop(true)
					(
						VOLT_MAKE_MODULE(UVolt_ASM_InterpWidgetTransform)
						.InterpolationMode(EVoltInterpMode::AlphaBased)
						.AlphaBasedEasingFunction(EEasingFunc::CircularInOut)
						.bUseStartWidgetTransform(true)
						.StartWidgetTransform(FWidgetTransform(FVector2D(-50, 0), FVector2D(1, 1), FVector2D(0, 0), 0))
						.TargetWidgetTransform(FWidgetTransform(FVector2D(50, 0), FVector2D(1, 1), FVector2D(0, 0), 0))
						.AlphaBasedBlendExp(2)
						.AlphaBasedDuration(3),
						VOLT_MAKE_MODULE(UVolt_ASM_InterpWidgetTransform)
						.InterpolationMode(EVoltInterpMode::AlphaBased)
						.AlphaBasedEasingFunction(EEasingFunc::CircularInOut)
						.bUseStartWidgetTransform(true)
						.StartWidgetTransform(FWidgetTransform(FVector2D(50, 0), FVector2D(1, 1), FVector2D(0, 0), 0))
						.TargetWidgetTransform(FWidgetTransform(FVector2D(-50, 0), FVector2D(1, 1), FVector2D(0, 0), 0))
						.AlphaBasedBlendExp(2)
						.AlphaBasedDuration(3)
					)
				)
			)
		]
		+ SWrapBox::Slot()
		.ForceNewLine(false)
		.Padding(VOLT_STARSHIP_BORDER_MARGIN)
		[
			SNew(SVoltInterpModuleBackground)
			.Title(INVTEXT("AlphaBased : EaseIn"))
			.AnimationManager(AnimationManager)
			.AnimationToPlay(
				VOLT_MAKE_ANIMATION(UVoltAnimation)(
					VOLT_MAKE_MODULE(UVolt_ASM_SetWidgetTransformPivot)
					.TargetWidgetTransformPivot(FVector2D(0.5f, 0.5f)),
					VOLT_MAKE_MODULE(UVolt_ASM_Sequence)
					.bShouldLoop(true)
					(
						VOLT_MAKE_MODULE(UVolt_ASM_InterpWidgetTransform)
						.InterpolationMode(EVoltInterpMode::AlphaBased)
						.AlphaBasedEasingFunction(EEasingFunc::EaseIn)
						.bUseStartWidgetTransform(true)
						.StartWidgetTransform(FWidgetTransform(FVector2D(-50, 0), FVector2D(1, 1), FVector2D(0, 0), 0))
						.TargetWidgetTransform(FWidgetTransform(FVector2D(50, 0), FVector2D(1, 1), FVector2D(0, 0), 0))
						.AlphaBasedBlendExp(2)
						.AlphaBasedDuration(3),
						VOLT_MAKE_MODULE(UVolt_ASM_InterpWidgetTransform)
						.InterpolationMode(EVoltInterpMode::AlphaBased)
						.AlphaBasedEasingFunction(EEasingFunc::EaseIn)
						.bUseStartWidgetTransform(true)
						.StartWidgetTransform(FWidgetTransform(FVector2D(50, 0), FVector2D(1, 1), FVector2D(0, 0), 0))
						.TargetWidgetTransform(FWidgetTransform(FVector2D(-50, 0), FVector2D(1, 1), FVector2D(0, 0), 0))
						.AlphaBasedBlendExp(2)
						.AlphaBasedDuration(3)
					)
				)
			)
		]
		+ SWrapBox::Slot()
		.ForceNewLine(false)
		.Padding(VOLT_STARSHIP_BORDER_MARGIN)
		[
			SNew(SVoltInterpModuleBackground)
			.Title(INVTEXT("AlphaBased : EaseOut"))
			.AnimationManager(AnimationManager)
			.AnimationToPlay(
				VOLT_MAKE_ANIMATION(UVoltAnimation)(
					VOLT_MAKE_MODULE(UVolt_ASM_SetWidgetTransformPivot)
					.TargetWidgetTransformPivot(FVector2D(0.5f, 0.5f)),
					VOLT_MAKE_MODULE(UVolt_ASM_Sequence)
					.bShouldLoop(true)
					(
						VOLT_MAKE_MODULE(UVolt_ASM_InterpWidgetTransform)
						.InterpolationMode(EVoltInterpMode::AlphaBased)
						.AlphaBasedEasingFunction(EEasingFunc::EaseOut)
						.bUseStartWidgetTransform(true)
						.StartWidgetTransform(FWidgetTransform(FVector2D(-50, 0), FVector2D(1, 1), FVector2D(0, 0), 0))
						.TargetWidgetTransform(FWidgetTransform(FVector2D(50, 0), FVector2D(1, 1), FVector2D(0, 0), 0))
						.AlphaBasedBlendExp(2)
						.AlphaBasedDuration(3),
						VOLT_MAKE_MODULE(UVolt_ASM_InterpWidgetTransform)
						.InterpolationMode(EVoltInterpMode::AlphaBased)
						.AlphaBasedEasingFunction(EEasingFunc::EaseOut)
						.bUseStartWidgetTransform(true)
						.StartWidgetTransform(FWidgetTransform(FVector2D(50, 0), FVector2D(1, 1), FVector2D(0, 0), 0))
						.TargetWidgetTransform(FWidgetTransform(FVector2D(-50, 0), FVector2D(1, 1), FVector2D(0, 0), 0))
						.AlphaBasedBlendExp(2)
						.AlphaBasedDuration(3)
					)
				)
			)
		]
		+ SWrapBox::Slot()
		.ForceNewLine(false)
		.Padding(VOLT_STARSHIP_BORDER_MARGIN)
		[
			SNew(SVoltInterpModuleBackground)
			.Title(INVTEXT("AlphaBased : EaseInOut"))
			.AnimationManager(AnimationManager)
			.AnimationToPlay(
				VOLT_MAKE_ANIMATION(UVoltAnimation)(
					VOLT_MAKE_MODULE(UVolt_ASM_SetWidgetTransformPivot)
					.TargetWidgetTransformPivot(FVector2D(0.5f, 0.5f)),
					VOLT_MAKE_MODULE(UVolt_ASM_Sequence)
					.bShouldLoop(true)
					(
						VOLT_MAKE_MODULE(UVolt_ASM_InterpWidgetTransform)
						.InterpolationMode(EVoltInterpMode::AlphaBased)
						.AlphaBasedEasingFunction(EEasingFunc::EaseInOut)
						.bUseStartWidgetTransform(true)
						.StartWidgetTransform(FWidgetTransform(FVector2D(-50, 0), FVector2D(1, 1), FVector2D(0, 0), 0))
						.TargetWidgetTransform(FWidgetTransform(FVector2D(50, 0), FVector2D(1, 1), FVector2D(0, 0), 0))
						.AlphaBasedBlendExp(2)
						.AlphaBasedDuration(3),
						VOLT_MAKE_MODULE(UVolt_ASM_InterpWidgetTransform)
						.InterpolationMode(EVoltInterpMode::AlphaBased)
						.AlphaBasedEasingFunction(EEasingFunc::EaseInOut)
						.bUseStartWidgetTransform(true)
						.StartWidgetTransform(FWidgetTransform(FVector2D(50, 0), FVector2D(1, 1), FVector2D(0, 0), 0))
						.TargetWidgetTransform(FWidgetTransform(FVector2D(-50, 0), FVector2D(1, 1), FVector2D(0, 0), 0))
						.AlphaBasedBlendExp(2)
						.AlphaBasedDuration(3)
					)
				)
			)
		]
		+ SWrapBox::Slot()
		.ForceNewLine(false)
		.Padding(VOLT_STARSHIP_BORDER_MARGIN)
		[
			SNew(SVoltInterpModuleBackground)
			.Title(INVTEXT("AlphaBased : ExpoIn"))
			.AnimationManager(AnimationManager)
			.AnimationToPlay(
				VOLT_MAKE_ANIMATION(UVoltAnimation)(
					VOLT_MAKE_MODULE(UVolt_ASM_SetWidgetTransformPivot)
					.TargetWidgetTransformPivot(FVector2D(0.5f, 0.5f)),
					VOLT_MAKE_MODULE(UVolt_ASM_Sequence)
					.bShouldLoop(true)
					(
						VOLT_MAKE_MODULE(UVolt_ASM_InterpWidgetTransform)
						.InterpolationMode(EVoltInterpMode::AlphaBased)
						.AlphaBasedEasingFunction(EEasingFunc::ExpoIn)
						.bUseStartWidgetTransform(true)
						.StartWidgetTransform(FWidgetTransform(FVector2D(-50, 0), FVector2D(1, 1), FVector2D(0, 0), 0))
						.TargetWidgetTransform(FWidgetTransform(FVector2D(50, 0), FVector2D(1, 1), FVector2D(0, 0), 0))
						.AlphaBasedBlendExp(2)
						.AlphaBasedDuration(3),
						VOLT_MAKE_MODULE(UVolt_ASM_InterpWidgetTransform)
						.InterpolationMode(EVoltInterpMode::AlphaBased)
						.AlphaBasedEasingFunction(EEasingFunc::ExpoIn)
						.bUseStartWidgetTransform(true)
						.StartWidgetTransform(FWidgetTransform(FVector2D(50, 0), FVector2D(1, 1), FVector2D(0, 0), 0))
						.TargetWidgetTransform(FWidgetTransform(FVector2D(-50, 0), FVector2D(1, 1), FVector2D(0, 0), 0))
						.AlphaBasedBlendExp(2)
						.AlphaBasedDuration(3)
					)
				)
			)
		]
		+ SWrapBox::Slot()
		.ForceNewLine(false)
		.Padding(VOLT_STARSHIP_BORDER_MARGIN)
		[
			SNew(SVoltInterpModuleBackground)
			.Title(INVTEXT("AlphaBased : ExpoOut"))
			.AnimationManager(AnimationManager)
			.AnimationToPlay(
				VOLT_MAKE_ANIMATION(UVoltAnimation)(
					VOLT_MAKE_MODULE(UVolt_ASM_SetWidgetTransformPivot)
					.TargetWidgetTransformPivot(FVector2D(0.5f, 0.5f)),
					VOLT_MAKE_MODULE(UVolt_ASM_Sequence)
					.bShouldLoop(true)
					(
						VOLT_MAKE_MODULE(UVolt_ASM_InterpWidgetTransform)
						.InterpolationMode(EVoltInterpMode::AlphaBased)
						.AlphaBasedEasingFunction(EEasingFunc::ExpoOut)
						.bUseStartWidgetTransform(true)
						.StartWidgetTransform(FWidgetTransform(FVector2D(-50, 0), FVector2D(1, 1), FVector2D(0, 0), 0))
						.TargetWidgetTransform(FWidgetTransform(FVector2D(50, 0), FVector2D(1, 1), FVector2D(0, 0), 0))
						.AlphaBasedBlendExp(2)
						.AlphaBasedDuration(3),
						VOLT_MAKE_MODULE(UVolt_ASM_InterpWidgetTransform)
						.InterpolationMode(EVoltInterpMode::AlphaBased)
						.AlphaBasedEasingFunction(EEasingFunc::ExpoOut)
						.bUseStartWidgetTransform(true)
						.StartWidgetTransform(FWidgetTransform(FVector2D(50, 0), FVector2D(1, 1), FVector2D(0, 0), 0))
						.TargetWidgetTransform(FWidgetTransform(FVector2D(-50, 0), FVector2D(1, 1), FVector2D(0, 0), 0))
						.AlphaBasedBlendExp(2)
						.AlphaBasedDuration(3)
					)
				)
			)
		]
		+ SWrapBox::Slot()
		.ForceNewLine(false)
		.Padding(VOLT_STARSHIP_BORDER_MARGIN)
		[
			SNew(SVoltInterpModuleBackground)
			.Title(INVTEXT("AlphaBased : ExpoInOut"))
			.AnimationManager(AnimationManager)
			.AnimationToPlay(
				VOLT_MAKE_ANIMATION(UVoltAnimation)(
					VOLT_MAKE_MODULE(UVolt_ASM_SetWidgetTransformPivot)
					.TargetWidgetTransformPivot(FVector2D(0.5f, 0.5f)),
					VOLT_MAKE_MODULE(UVolt_ASM_Sequence)
					.bShouldLoop(true)
					(
						VOLT_MAKE_MODULE(UVolt_ASM_InterpWidgetTransform)
						.InterpolationMode(EVoltInterpMode::AlphaBased)
						.AlphaBasedEasingFunction(EEasingFunc::ExpoInOut)
						.bUseStartWidgetTransform(true)
						.StartWidgetTransform(FWidgetTransform(FVector2D(-50, 0), FVector2D(1, 1), FVector2D(0, 0), 0))
						.TargetWidgetTransform(FWidgetTransform(FVector2D(50, 0), FVector2D(1, 1), FVector2D(0, 0), 0))
						.AlphaBasedBlendExp(2)
						.AlphaBasedDuration(3),
						VOLT_MAKE_MODULE(UVolt_ASM_InterpWidgetTransform)
						.InterpolationMode(EVoltInterpMode::AlphaBased)
						.AlphaBasedEasingFunction(EEasingFunc::ExpoInOut)
						.bUseStartWidgetTransform(true)
						.StartWidgetTransform(FWidgetTransform(FVector2D(50, 0), FVector2D(1, 1), FVector2D(0, 0), 0))
						.TargetWidgetTransform(FWidgetTransform(FVector2D(-50, 0), FVector2D(1, 1), FVector2D(0, 0), 0))
						.AlphaBasedBlendExp(2)
						.AlphaBasedDuration(3)
					)
				)
			)
		]
		+ SWrapBox::Slot()
		.ForceNewLine(false)
		.Padding(VOLT_STARSHIP_BORDER_MARGIN)
		[
			SNew(SVoltInterpModuleBackground)
			.Title(INVTEXT("AlphaBased : SinusoidalIn"))
			.AnimationManager(AnimationManager)
			.AnimationToPlay(
				VOLT_MAKE_ANIMATION(UVoltAnimation)(
					VOLT_MAKE_MODULE(UVolt_ASM_SetWidgetTransformPivot)
					.TargetWidgetTransformPivot(FVector2D(0.5f, 0.5f)),
					VOLT_MAKE_MODULE(UVolt_ASM_Sequence)
					.bShouldLoop(true)
					(
						VOLT_MAKE_MODULE(UVolt_ASM_InterpWidgetTransform)
						.InterpolationMode(EVoltInterpMode::AlphaBased)
						.AlphaBasedEasingFunction(EEasingFunc::SinusoidalIn)
						.bUseStartWidgetTransform(true)
						.StartWidgetTransform(FWidgetTransform(FVector2D(-50, 0), FVector2D(1, 1), FVector2D(0, 0), 0))
						.TargetWidgetTransform(FWidgetTransform(FVector2D(50, 0), FVector2D(1, 1), FVector2D(0, 0), 0))
						.AlphaBasedBlendExp(2)
						.AlphaBasedDuration(3),
						VOLT_MAKE_MODULE(UVolt_ASM_InterpWidgetTransform)
						.InterpolationMode(EVoltInterpMode::AlphaBased)
						.AlphaBasedEasingFunction(EEasingFunc::SinusoidalIn)
						.bUseStartWidgetTransform(true)
						.StartWidgetTransform(FWidgetTransform(FVector2D(50, 0), FVector2D(1, 1), FVector2D(0, 0), 0))
						.TargetWidgetTransform(FWidgetTransform(FVector2D(-50, 0), FVector2D(1, 1), FVector2D(0, 0), 0))
						.AlphaBasedBlendExp(2)
						.AlphaBasedDuration(3)
					)
				)
			)
		]
		+ SWrapBox::Slot()
		.ForceNewLine(false)
		.Padding(VOLT_STARSHIP_BORDER_MARGIN)
		[
			SNew(SVoltInterpModuleBackground)
			.Title(INVTEXT("AlphaBased : SinusoidalOut"))
			.AnimationManager(AnimationManager)
			.AnimationToPlay(
				VOLT_MAKE_ANIMATION(UVoltAnimation)(
					VOLT_MAKE_MODULE(UVolt_ASM_SetWidgetTransformPivot)
					.TargetWidgetTransformPivot(FVector2D(0.5f, 0.5f)),
					VOLT_MAKE_MODULE(UVolt_ASM_Sequence)
					.bShouldLoop(true)
					(
						VOLT_MAKE_MODULE(UVolt_ASM_InterpWidgetTransform)
						.InterpolationMode(EVoltInterpMode::AlphaBased)
						.AlphaBasedEasingFunction(EEasingFunc::SinusoidalOut)
						.bUseStartWidgetTransform(true)
						.StartWidgetTransform(FWidgetTransform(FVector2D(-50, 0), FVector2D(1, 1), FVector2D(0, 0), 0))
						.TargetWidgetTransform(FWidgetTransform(FVector2D(50, 0), FVector2D(1, 1), FVector2D(0, 0), 0))
						.AlphaBasedBlendExp(2)
						.AlphaBasedDuration(3),
						VOLT_MAKE_MODULE(UVolt_ASM_InterpWidgetTransform)
						.InterpolationMode(EVoltInterpMode::AlphaBased)
						.AlphaBasedEasingFunction(EEasingFunc::SinusoidalOut)
						.bUseStartWidgetTransform(true)
						.StartWidgetTransform(FWidgetTransform(FVector2D(50, 0), FVector2D(1, 1), FVector2D(0, 0), 0))
						.TargetWidgetTransform(FWidgetTransform(FVector2D(-50, 0), FVector2D(1, 1), FVector2D(0, 0), 0))
						.AlphaBasedBlendExp(2)
						.AlphaBasedDuration(3)
					)
				)
			)
		]
		+ SWrapBox::Slot()
		.ForceNewLine(false)
		.Padding(VOLT_STARSHIP_BORDER_MARGIN)
		[
			SNew(SVoltInterpModuleBackground)
			.Title(INVTEXT("AlphaBased : SinusoidalInOut"))
			.AnimationManager(AnimationManager)
			.AnimationToPlay(
				VOLT_MAKE_ANIMATION(UVoltAnimation)(
					VOLT_MAKE_MODULE(UVolt_ASM_SetWidgetTransformPivot)
					.TargetWidgetTransformPivot(FVector2D(0.5f, 0.5f)),
					VOLT_MAKE_MODULE(UVolt_ASM_Sequence)
					.bShouldLoop(true)
					(
						VOLT_MAKE_MODULE(UVolt_ASM_InterpWidgetTransform)
						.InterpolationMode(EVoltInterpMode::AlphaBased)
						.AlphaBasedEasingFunction(EEasingFunc::SinusoidalInOut)
						.bUseStartWidgetTransform(true)
						.StartWidgetTransform(FWidgetTransform(FVector2D(-50, 0), FVector2D(1, 1), FVector2D(0, 0), 0))
						.TargetWidgetTransform(FWidgetTransform(FVector2D(50, 0), FVector2D(1, 1), FVector2D(0, 0), 0))
						.AlphaBasedBlendExp(2)
						.AlphaBasedDuration(3),
						VOLT_MAKE_MODULE(UVolt_ASM_InterpWidgetTransform)
						.InterpolationMode(EVoltInterpMode::AlphaBased)
						.AlphaBasedEasingFunction(EEasingFunc::SinusoidalInOut)
						.bUseStartWidgetTransform(true)
						.StartWidgetTransform(FWidgetTransform(FVector2D(50, 0), FVector2D(1, 1), FVector2D(0, 0), 0))
						.TargetWidgetTransform(FWidgetTransform(FVector2D(-50, 0), FVector2D(1, 1), FVector2D(0, 0), 0))
						.AlphaBasedBlendExp(2)
						.AlphaBasedDuration(3)
					)
				)
			)
		];

	return Box;
}


void SVoltSampleStarshipTab::Construct(const FArguments& InArgs)
{
	VOLT_IMPLEMENT_MANAGER(&AnimationManager, SharedThis(this));

	ChildSlot
		.VAlign(VAlign_Fill)
		.HAlign(HAlign_Fill)
		[
			SNew(SScrollBox)
			.AllowOverscroll(EAllowOverscroll::Yes)
			.AnimateWheelScrolling(true)
			+ SScrollBox::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Center)
			.Padding(VOLT_STARSHIP_BORDER_MARGIN)
			[
				SNew(STextBlock)
				.Text(INVTEXT("Usecases"))
				.TextStyle(FVoltEditorStyle::Get(), "VoltStarship.Font.White.Big")
			]
			+ SScrollBox::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Center)
			.Padding(VOLT_STARSHIP_BORDER_MARGIN)
			[
				SNew(STextBlock)
				.Text(INVTEXT(
					"The power of Volt isn't about the animation itself, but it's the convienience that makes the animation implementation tangible. You can still animate your slates with original slate system, but it's very difficult or even impossible to achieve, but with Volt, Everyone can animate their slates as they want."))
				.TextStyle(FVoltEditorStyle::Get(), "VoltStarship.Font.White")
				.AutoWrapText(true)
			]
			+ SScrollBox::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Center)
			.Padding(VOLT_STARSHIP_BORDER_MARGIN)
			[
				SNew(STextBlock)
				.Text(INVTEXT(
					"You can use the slates here as start point of your journey, but we genuienly don't recommend you to use the slates in this tab directly in your project - We didn't have enough time to refactor or implement all the features that each slate type must provide or test it out further."))
				.TextStyle(FVoltEditorStyle::Get(), "VoltStarship.Font.White")
				.AutoWrapText(true)
			]
			+ SScrollBox::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Center)
			.Padding(VOLT_STARSHIP_BORDER_MARGIN)
			[
				SNew(STextBlock)
				.Text(INVTEXT(
					"If any cool guy shows up and make an additional slate library for it, we will appriciate it a ton!"))
				.TextStyle(FVoltEditorStyle::Get(), "VoltStarship.Font.White")
				.AutoWrapText(true)
			]
			+ SScrollBox::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Center)
			.Padding(VOLT_STARSHIP_BORDER_MARGIN)
			[
				SNew(STextBlock)
				.Text(INVTEXT("Buttons - For basic usages. See SVoltStarshipButton"))
				.TextStyle(FVoltEditorStyle::Get(), "VoltStarship.Font.White")
			]
			+ SScrollBox::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Center)
			.Padding(VOLT_STARSHIP_BORDER_MARGIN)
			[
				CreateButtonSection()
			]
			+ SScrollBox::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Center)
			.Padding(VOLT_STARSHIP_BORDER_MARGIN)
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.HAlign(HAlign_Left)
				.VAlign(VAlign_Center)
				[
					SNew(STextBlock)
					.Text(INVTEXT("ScrollBox - Leveled animation and per element animation"))
					.TextStyle(FVoltEditorStyle::Get(), "VoltStarship.Font.White")
				]
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.HAlign(HAlign_Left)
				.VAlign(VAlign_Center)
				.Padding(FMargin(4, 0))
				[
					SNew(SVoltStarshipButton)
					.AnimationManager(AnimationManager)
					.OutlineBorderImage(FVoltEditorStyle::Get().GetBrush("VoltStarship.Border.Rounder"))
					.ButtonStyle(FVoltEditorStyle::Get(), "VoltStarship.Button.Rounder")
					.ContentMargin(FMargin(10))
					.OnPressed(this, &SVoltSampleStarshipTab::ResetScrollSection)
					[
						SNew(STextBlock)
						.Text(INVTEXT("Reset Scroll"))
					]
				]
			]
			+ SScrollBox::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Center)
			.Padding(VOLT_STARSHIP_BORDER_MARGIN)
			[
				CreateScrollSection()
			]
			+ SScrollBox::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Center)
			.Padding(VOLT_STARSHIP_BORDER_MARGIN)
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.HAlign(HAlign_Left)
				.VAlign(VAlign_Center)
				[
					SNew(STextBlock)
					.Text(INVTEXT(
						"Description Card - Good for tooltips, or simple descriptions. Or maybe even as a christmas card?"))
					.TextStyle(FVoltEditorStyle::Get(), "VoltStarship.Font.White")
				]
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.HAlign(HAlign_Left)
				.VAlign(VAlign_Center)
				.Padding(FMargin(4, 0))
				[
					SNew(SVoltStarshipButton)
					.AnimationManager(AnimationManager)
					.OutlineBorderImage(FVoltEditorStyle::Get().GetBrush("VoltStarship.Border.Rounder"))
					.ButtonStyle(FVoltEditorStyle::Get(), "VoltStarship.Button.Rounder")
					.ContentMargin(FMargin(10))
					.OnPressed(this, &SVoltSampleStarshipTab::ResetCard)
					[
						SNew(STextBlock)
						.Text(INVTEXT("Reset Card"))
					]
				]
			]
			+ SScrollBox::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Center)
			.Padding(VOLT_STARSHIP_BORDER_MARGIN)
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				.Padding(FMargin(20))
				[
					CreateDescriptionSection()
				]
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				.Padding(FMargin(20))
				[
					SNew(SVoltStarshipBorder)
					.AnimationManager(AnimationManager)
					.ContentMargin(FMargin(10))
					.OutlineMargin(FMargin(1))
					.InnerBorderImage(FVoltEditorStyle::Get().GetBrush("VoltStarship.Border.Round"))
					.OuterBorderImage(FVoltEditorStyle::Get().GetBrush("VoltStarship.Border.Round"))
					[

						SNew(SVerticalBox)
						+ SVerticalBox::Slot()
						.AutoHeight()
						.VAlign(VAlign_Center)
						.HAlign(HAlign_Center)
						.Padding(FMargin(20))
						[
							SNew(STextBlock)
							.Text(INVTEXT("Put your cursor here and see the widget in a tooltip."))
						]
						+ SVerticalBox::Slot()
						.AutoHeight()
						.VAlign(VAlign_Center)
						.HAlign(HAlign_Center)
						.Padding(FMargin(20))
						[
							SNew(SVoltStarshipButton)
							.AnimationManager(AnimationManager)
							.OutlineBorderImage(FVoltEditorStyle::Get().GetBrush("VoltStarship.Border.Round"))
							.ButtonStyle(FVoltEditorStyle::Get(), "VoltStarship.Button.Round")
							.ToolTip(
								SNew(SVoltStarshipTooltip)
								.OnVoltStarshipTooltipOpened(this, &SVoltSampleStarshipTab::Callback_Tooltip_Reset)
								[
									SAssignNew(TooltipDescriptionCard, SVoltStarshipDescriptionCard)
									.Title(INVTEXT("VOLT Library"))
									.Description(INVTEXT(
										"Volt is a slate architecture animation library for Unreal Engine. It supports unique declarative syntax that let you animate slates in split seconds."))
									.AnimationManager(AnimationManager)
									.Size(FVector2D(500, 250))
									.ExplanationSectionColor(FColor(110, 199, 255))
									[
										SNew(STextBlock)
										.Text(INVTEXT("VOLT"))
										.TextStyle(FVoltEditorStyle::Get(), "VoltStarship.Font.White.Biggest")
										.ColorAndOpacity(FLinearColor::White)
									]
								]
							)
							.ContentMargin(FMargin(10))
							[
								SNew(STextBlock)
								.Text(INVTEXT("Put your cursor here!"))
							]
						]
					]
				]
			]
			+ SScrollBox::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Center)
			.Padding(VOLT_STARSHIP_BORDER_MARGIN)
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.HAlign(HAlign_Left)
				.VAlign(VAlign_Center)
				[
					SNew(STextBlock)
					.Text(INVTEXT("Naturally expanding and appearing items with Box size animation"))
					.TextStyle(FVoltEditorStyle::Get(), "VoltStarship.Font.White")
				]
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.HAlign(HAlign_Left)
				.VAlign(VAlign_Center)
				.Padding(FMargin(4, 0))
				[
					SNew(SVoltStarshipButton)
					.AnimationManager(AnimationManager)
					.OutlineBorderImage(FVoltEditorStyle::Get().GetBrush("VoltStarship.Border.Rounder"))
					.ButtonStyle(FVoltEditorStyle::Get(), "VoltStarship.Button.Rounder")
					.ContentMargin(FMargin(10))
					.OnPressed(this, &SVoltSampleStarshipTab::ResetExpandingBorderSection)
					[
						SNew(STextBlock)
						.Text(INVTEXT("Reset Appearing Item Animation."))
					]
				]
			]
			+ SScrollBox::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Center)
			.Padding(VOLT_STARSHIP_BORDER_MARGIN)
			[
				CreateExpandingBorderSection()
			]
		];
}

TSharedRef<SWidget> SVoltSampleStarshipTab::CreateButtonSection()
{
	TSharedRef<SWrapBox> Box = SNew(SWrapBox)
		.UseAllottedSize(true)
		.Orientation(Orient_Horizontal)
		+ SWrapBox::Slot()
		.ForceNewLine(false)
		.Padding(VOLT_STARSHIP_BORDER_MARGIN)
		[
			SNew(SVoltStarshipBorder)
			.AnimationManager(AnimationManager)
			.ContentMargin(FMargin(10))
			.OutlineMargin(FMargin(1))
			.InnerBorderImage(FVoltEditorStyle::Get().GetBrush("VoltStarship.Border.Round"))
			.OuterBorderImage(FVoltEditorStyle::Get().GetBrush("VoltStarship.Border.Round"))
			[

				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.AutoHeight()
				.VAlign(VAlign_Center)
				.HAlign(HAlign_Center)
				.Padding(FMargin(20))
				[
					SNew(STextBlock)
					.Text(INVTEXT("RoundConner"))
				]
				+ SVerticalBox::Slot()
				.AutoHeight()
				.VAlign(VAlign_Center)
				.HAlign(HAlign_Center)
				.Padding(FMargin(20))
				[
					SNew(SVoltStarshipButton)
					.AnimationManager(AnimationManager)
					.OutlineBorderImage(FVoltEditorStyle::Get().GetBrush("VoltStarship.Border.Round"))
					.ButtonStyle(FVoltEditorStyle::Get(), "VoltStarship.Button.Round")
					.ContentMargin(FMargin(10))
					[
						SNew(STextBlock)
						.Text(INVTEXT("Press Me!"))
					]
				]
			]
		]
		+ SWrapBox::Slot()
		.ForceNewLine(false)
		.Padding(VOLT_STARSHIP_BORDER_MARGIN)
		[
			SNew(SVoltStarshipBorder)
			.AnimationManager(AnimationManager)
			.ContentMargin(FMargin(10))
			.OutlineMargin(FMargin(1))
			.InnerBorderImage(FVoltEditorStyle::Get().GetBrush("VoltStarship.Border.Round"))
			.OuterBorderImage(FVoltEditorStyle::Get().GetBrush("VoltStarship.Border.Round"))
			[

				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.AutoHeight()
				.VAlign(VAlign_Center)
				.HAlign(HAlign_Center)
				.Padding(FMargin(20))
				[
					SNew(STextBlock)
					.Text(INVTEXT("Round"))
				]
				+ SVerticalBox::Slot()
				.AutoHeight()
				.VAlign(VAlign_Center)
				.HAlign(HAlign_Center)
				.Padding(FMargin(20))
				[
					SNew(SVoltStarshipButton)
					.AnimationManager(AnimationManager)
					.OutlineBorderImage(FVoltEditorStyle::Get().GetBrush("VoltStarship.Border.Sphere"))
					.ButtonStyle(FVoltEditorStyle::Get(), "VoltStarship.Button.Sphere")
					.ContentMargin(FMargin(10))
					[
						SNew(STextBlock)
						.Text(INVTEXT("Press Me!"))
					]
				]
				+ SVerticalBox::Slot()
				.AutoHeight()
				.VAlign(VAlign_Center)
				.HAlign(HAlign_Center)
				.Padding(FMargin(20))
				[
					SNew(SVoltStarshipButton)
					.AnimationManager(AnimationManager)
					.OutlineBorderImage(FVoltEditorStyle::Get().GetBrush("VoltStarship.Border.Sphere"))
					.ButtonStyle(FVoltEditorStyle::Get(), "VoltStarship.Button.Sphere")
					.ContentMargin(FMargin(10))
					.NormalColor(FColor(107, 70, 95))
					.HoverColor(FColor(207, 68, 94))
					.PressColor(FColor(255, 138, 36))
					.OutlineNormalColor(FColor(255, 252, 222))
					.OutlineHoverColor(FColor(255, 194, 224))
					.OutlinePressColor(FColor(255,255,255))
					[
						SNew(STextBlock)
						.Text(INVTEXT("Colors are customizable."))
					]
				]
			]
		]
		+ SWrapBox::Slot()
		.ForceNewLine(false)
		.Padding(VOLT_STARSHIP_BORDER_MARGIN)
		[
			SNew(SVoltStarshipBorder)
			.AnimationManager(AnimationManager)
			.ContentMargin(FMargin(10))
			.OutlineMargin(FMargin(1))
			.InnerBorderImage(FVoltEditorStyle::Get().GetBrush("VoltStarship.Border.Round"))
			.OuterBorderImage(FVoltEditorStyle::Get().GetBrush("VoltStarship.Border.Round"))
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.AutoHeight()
				.VAlign(VAlign_Center)
				.HAlign(HAlign_Center)
				.Padding(FMargin(20))
				[
					SNew(STextBlock)
					.Text(INVTEXT("A Solid Color design. Yeah, why not?"))
				]
				+ SVerticalBox::Slot()
				.AutoHeight()
				.VAlign(VAlign_Center)
				.HAlign(HAlign_Center)
				.Padding(FMargin(20))
				[
					SNew(SVoltStarshipButton)
					.AnimationManager(AnimationManager)
					.OutlineBorderImage(FVoltEditorStyle::Get().GetBrush("VoltStarship.Border.Sphere"))
					.ButtonStyle(FVoltEditorStyle::Get(), "VoltStarship.Button.Sphere")
					.NormalColor(FColor(63, 161, 209))
					.HoverColor(FColor(78, 171, 217))
					.PressColor(FColor(53, 122, 156))
					.ContentMargin(FMargin(10))
					.OutlineNormalColor(FLinearColor::Transparent)
					.OutlineHoverColor(FLinearColor::Transparent)
					.OutlinePressColor(FLinearColor::Transparent)
					[
						SNew(STextBlock)
						.Text(INVTEXT("Press Me!"))
					]
				]
			]
		]
		+ SWrapBox::Slot()
		.ForceNewLine(false)
		.Padding(VOLT_STARSHIP_BORDER_MARGIN)
		[
			SNew(SVoltStarshipBorder)
			.AnimationManager(AnimationManager)
			.ContentMargin(FMargin(10))
			.OutlineMargin(FMargin(1))
			.InnerBorderImage(FVoltEditorStyle::Get().GetBrush("VoltStarship.Border.Round"))
			.OuterBorderImage(FVoltEditorStyle::Get().GetBrush("VoltStarship.Border.Round"))
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.AutoHeight()
				.VAlign(VAlign_Center)
				.HAlign(HAlign_Center)
				.Padding(FMargin(20))
				[
					SNew(STextBlock)
					.Text(INVTEXT("Clean & transparent text button"))
				]
				+ SVerticalBox::Slot()
				.AutoHeight()
				.VAlign(VAlign_Center)
				.HAlign(HAlign_Center)
				.Padding(FMargin(20))
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.VAlign(VAlign_Center)
					.HAlign(HAlign_Center)
					.Padding(FMargin(1))
					[
						SNew(SVoltStarshipButton)
						.AnimationManager(AnimationManager)
						.OutlineBorderImage(FVoltEditorStyle::Get().GetBrush("VoltStarship.Border.Sphere"))
						.ButtonStyle(FVoltEditorStyle::Get(), "VoltStarship.Button.Sphere")
						.NormalColor(FLinearColor::Transparent)
						.HoverColor(FLinearColor(0.2, 0.2, 0.2, 1))
						.PressColor(FLinearColor(0.4, 0.4, 0.4, 1))
						.OutlineNormalColor(FLinearColor::Transparent)
						.OutlineHoverColor(FLinearColor::Transparent)
						.OutlinePressColor(FLinearColor::Transparent)
						.ContentMargin(FMargin(10))
						[
							SNew(STextBlock)
							.Text(INVTEXT("Text"))
						]
					]
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.VAlign(VAlign_Center)
					.HAlign(HAlign_Center)
					.Padding(FMargin(1))
					[
						SNew(SVoltStarshipButton)
						.AnimationManager(AnimationManager)
						.OutlineBorderImage(FVoltEditorStyle::Get().GetBrush("VoltStarship.Border.Sphere"))
						.ButtonStyle(FVoltEditorStyle::Get(), "VoltStarship.Button.Sphere")
						.NormalColor(FLinearColor::Transparent)
						.HoverColor(FLinearColor(0.2, 0.2, 0.2, 1))
						.PressColor(FLinearColor(0.4, 0.4, 0.4, 1))
						.OutlineNormalColor(FLinearColor::Transparent)
						.OutlineHoverColor(FLinearColor::Transparent)
						.OutlinePressColor(FLinearColor::Transparent)
						.ContentMargin(FMargin(10))
						[
							SNew(STextBlock)
							.Text(INVTEXT("Like"))
						]
					]
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.VAlign(VAlign_Center)
					.HAlign(HAlign_Center)
					.Padding(FMargin(1))
					[
						SNew(SVoltStarshipButton)
						.AnimationManager(AnimationManager)
						.OutlineBorderImage(FVoltEditorStyle::Get().GetBrush("VoltStarship.Border.Sphere"))
						.ButtonStyle(FVoltEditorStyle::Get(), "VoltStarship.Button.Sphere")
						.NormalColor(FLinearColor::Transparent)
						.HoverColor(FLinearColor(0.2, 0.2, 0.2, 1))
						.PressColor(FLinearColor(0.4, 0.4, 0.4, 1))
						.OutlineNormalColor(FLinearColor::Transparent)
						.OutlineHoverColor(FLinearColor::Transparent)
						.OutlinePressColor(FLinearColor::Transparent)
						.ContentMargin(FMargin(10))
						[
							SNew(STextBlock)
							.Text(INVTEXT("Button!"))
						]
					]
				]
			]
		]
		+ SWrapBox::Slot()
		.ForceNewLine(false)
		.Padding(VOLT_STARSHIP_BORDER_MARGIN)
		[
			SNew(SVoltStarshipBorder)
			.AnimationManager(AnimationManager)
			.ContentMargin(FMargin(10))
			.OutlineMargin(FMargin(1))
			.InnerBorderImage(FVoltEditorStyle::Get().GetBrush("VoltStarship.Border.Round"))
			.OuterBorderImage(FVoltEditorStyle::Get().GetBrush("VoltStarship.Border.Round"))
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.AutoHeight()
				.VAlign(VAlign_Center)
				.HAlign(HAlign_Center)
				.Padding(FMargin(20))
				[
					SNew(STextBlock)
					.Text(INVTEXT("Good looking toggle button? here it is."))
				]
				+ SVerticalBox::Slot()
				.AutoHeight()
				.VAlign(VAlign_Center)
				.HAlign(HAlign_Center)
				.Padding(FMargin(20))
				[
					SNew(SVoltStarshipToggleButton)
					.AnimationManager(AnimationManager)
					.ButtonLength(20)
					.ButtonStyle(FVoltEditorStyle::Get(), "VoltStarship.Button.Sphere")
					.ToggleImage(FVoltEditorStyle::Get().GetBrush("VoltStarship.Image.Sphere"))
				]
			]
		];

	return Box;
}

TSharedRef<SWidget> SVoltSampleStarshipTab::CreateExpandingBorderSection()
{
	ExpandingBorderBox = SNew(SBox)
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		.HeightOverride(300);

	ResetExpandingBorderSection();
	
	return ExpandingBorderBox.ToSharedRef();
}

void SVoltSampleStarshipTab::ResetExpandingBorderSection()
{

	ExpandingBorderBox->SetContent(
			SNew(SVoltStarshipBorder)
			.AnimationManager(AnimationManager)
			.InnerBorderImage(FVoltEditorStyle::Get().GetBrush("VoltStarship.Border.Rounder"))
			.OuterBorderImage(FVoltEditorStyle::Get().GetBrush("VoltStarship.Border.Rounder"))
			.ContentMargin(10)
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.VAlign(VAlign_Center)
					.HAlign(HAlign_Left)
					[
						SNew(SVoltStarshipExpandingBorder)
						.AnimationManager(AnimationManager)
						.DelayDuration(1)
						[
							SNew(SBox)
							.MinDesiredHeight(18)
							[
								SNew(STextBlock)
								.Text(INVTEXT("Appearing items with dynamic expanding Border!!"))
							]
						]
					]
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.VAlign(VAlign_Center)
					.HAlign(HAlign_Left)
					[
						SNew(SVoltStarshipExpandingBorder)
						.AnimationManager(AnimationManager)
						.DelayDuration(3)
						[
							SNew(SBox)
							.MinDesiredHeight(18)
							[
								SNew(STextBlock)
								.Text(INVTEXT(" Not only for the texts...."))
							]
						]
					]
				]
				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.VAlign(VAlign_Center)
					.HAlign(HAlign_Left)
					[
						SNew(SVoltStarshipExpandingBorder)
						.AnimationManager(AnimationManager)
						.DelayDuration(5)
						[
							SNew(SImage)
							.Image(FVoltEditorStyle::GetUEEditorSlateStyleSet().GetBrush("UnrealDefaultThumbnail"))
							.DesiredSizeOverride(FVector2D(50,50))
						]
					]
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.VAlign(VAlign_Center)
					.HAlign(HAlign_Left)
					[
						SNew(SVoltStarshipExpandingBorder)
						.AnimationManager(AnimationManager)
						.DelayDuration(7)
						[
							SNew(SBox)
							.MinDesiredHeight(18)
							[
								SNew(STextBlock)
								.Text(INVTEXT(" Yeah, it's crazy, right? you can put everything here."))
							]
						]
					]
				]
				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.VAlign(VAlign_Center)
					.HAlign(HAlign_Left)
					[
						SNew(SVoltStarshipExpandingBorder)
						.AnimationManager(AnimationManager)
						.DelayDuration(10)
						[
							SNew(SBox)
							.MinDesiredHeight(18)
							[
								SNew(STextBlock)
								.Text(INVTEXT("The best part is what you have to do is just wrapping your slates in SVoltStarshipExpandingBorder and set the delay... "))
							]
						]
					]
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.VAlign(VAlign_Center)
					.HAlign(HAlign_Left)
					[
						SNew(SVoltStarshipExpandingBorder)
						.AnimationManager(AnimationManager)
						.DelayDuration(14)
						[
							SNew(SBox)
							.MinDesiredHeight(18)
							[
								SNew(STextBlock)
								.Text(INVTEXT("then boom!"))
								.TextStyle(FVoltEditorStyle::Get(), "VoltStarship.Font.White.Big")
							]
						]
					]
				]
				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.VAlign(VAlign_Center)
					.HAlign(HAlign_Left)
					[
						SNew(SVoltStarshipExpandingBorder)
						.AnimationManager(AnimationManager)
						.DelayDuration(17)
						[
							SNew(SBox)
							.MinDesiredHeight(18)
							[
								SNew(STextBlock)
								.Text(INVTEXT("Think about making something like this without Volt."))
							]
						]
					]
				]
				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.VAlign(VAlign_Center)
					.HAlign(HAlign_Left)
					[
						SNew(SVoltStarshipExpandingBorder)
						.AnimationManager(AnimationManager)
						.DelayDuration(19)
						[
							SNew(SBox)
							.MinDesiredHeight(18)
							[
								SNew(STextBlock)
								.Text(INVTEXT("It will be "))
							]
						]
					]
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.VAlign(VAlign_Center)
					.HAlign(HAlign_Left)
					[
						SNew(SVoltStarshipExpandingBorder)
						.AnimationManager(AnimationManager)
						.DelayDuration(21)
						[
							SNew(SBox)
							.MinDesiredHeight(18)
							[
								SNew(STextBlock)
								.Text(INVTEXT("very very very very very very "))
							]
						]
					]
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.VAlign(VAlign_Center)
					.HAlign(HAlign_Left)
					[
						SNew(SVoltStarshipExpandingBorder)
						.AnimationManager(AnimationManager)
						.DelayDuration(19)
						[
							SNew(SBox)
							.MinDesiredHeight(18)
							[
								SNew(STextBlock)
								.Text(INVTEXT("difficult and time-consuming."))
							]
						]
					]
				]
			]);
}

TSharedRef<SWidget> SVoltSampleStarshipTab::CreateScrollSection()
{
	TSharedRef<SBox> Box = SNew(SBox)
		.HeightOverride(300)
		[
			SAssignNew(ScrollBox, SScrollBox)
			.Orientation(Orient_Vertical)
			.AnimateWheelScrolling(true)
			.AllowOverscroll(EAllowOverscroll::Yes)
		];

	ResetScrollSection();

	return Box;
}

void SVoltSampleStarshipTab::ResetScrollSection()
{
	ScrollBox->ClearChildren();

	for (int i = 0; i < 10; ++i)
	{
		ScrollBox->AddSlot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(SVoltStarshipScrollElement)
				.Interval(i * 0.4)
				.AnimationManager(AnimationManager)
				.OutlineBorderImage(FVoltEditorStyle::Get().GetBrush("VoltStarship.Border.Rounder"))
				[
					SNew(SVoltStarshipBorder)
					.AnimationManager(AnimationManager)
					.InnerBorderImage(FVoltEditorStyle::Get().GetBrush("VoltStarship.Border.Rounder"))
					.OuterBorderImage(FVoltEditorStyle::Get().GetBrush("VoltStarship.Border.Rounder"))
					.HoverTransform(FWidgetTransform(FVector2D(-70, 0), FVector2D(1.2, 1.2), FVector2D(0, 0), 0))
					.HoverOpacity(1)
					.NormalOpacity(0.5)
					.ContentMargin(10)
					[
						SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
						.AutoWidth()
						.Padding(2)
						.HAlign(HAlign_Center)
						.VAlign(VAlign_Center)
						[
							SNew(STextBlock)
							.Text(INVTEXT("Good-looking-ScrollBox!"))
						]
						+ SHorizontalBox::Slot()
						.AutoWidth()
						.Padding(2)
						.HAlign(HAlign_Center)
						.VAlign(VAlign_Center)
						[
							SNew(SVoltStarshipButton)
							.AnimationManager(AnimationManager)
							.OutlineBorderImage(FVoltEditorStyle::Get().GetBrush("VoltStarship.Border.Rounder"))
							.ButtonStyle(FVoltEditorStyle::Get(), "VoltStarship.Button.Rounder")
							.NormalColor(FLinearColor::Transparent)
							.HoverColor(FLinearColor(0.2, 0.2, 0.2, 1))
							.PressColor(FLinearColor(0.4, 0.4, 0.4, 1))
							.OutlineNormalColor(FLinearColor::Transparent)
							.OutlineHoverColor(FLinearColor::Transparent)
							.OutlinePressColor(FLinearColor::Transparent)
							.ContentMargin(FMargin(10))
							[
								SNew(STextBlock)
								.Text(INVTEXT("Text"))
							]
						]
					]
				]
			];
	}
}

TSharedRef<SWidget> SVoltSampleStarshipTab::CreateDescriptionSection()
{
	DescriptionCardBox = SNew(SBox)
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		.HeightOverride(300);

	ResetCard();

	return DescriptionCardBox.ToSharedRef();
}

void SVoltSampleStarshipTab::ResetCard()
{
	DescriptionCardBox->SetContent(
		SNew(SVoltStarshipDescriptionCard)
		.Title(INVTEXT("VOLT Library"))
		.Description(INVTEXT(
			"Volt is a slate architecture animation library for Unreal Engine. It supports unique declarative syntax that let you animate slates in split seconds."))
		.AnimationManager(AnimationManager)
		.Size(FVector2D(500, 250))
		.ExplanationSectionColor(FColor(110, 199, 255))
		[
			SNew(STextBlock)
			.Text(INVTEXT("VOLT"))
			.TextStyle(FVoltEditorStyle::Get(), "VoltStarship.Font.White.Biggest")
			.ColorAndOpacity(FLinearColor::White)
		]
	);
}

void SVoltSampleStarshipTab::Callback_Tooltip_Reset(TSharedRef<SVoltStarshipTooltip> Tooltip)
{
	TooltipDescriptionCard->PlayInitAnimation();
}


#undef INVTEXT_NAMESPACE
#undef VOLT_STARSHIP_BORDER_MARGIN
#undef VOLT_STARSHIP_BUTTON_CONTENT_MARGIN

#undef VOLT_STARSHIP_BORDER_BACKGROUND_COLOR
#undef VOLT_STARSHIP_BORDER_BACKGROUND_COLOR_BRIGHT
#undef VOLT_STARSHIP_BORDER_BACKGROUND_COLOR_BRIGHT_HOVER
