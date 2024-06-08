//Copyright 2022~2024 DevGrain. All Rights Reserved.

#include "..\Public\VoltEditorStyle.h"
#include "Styling/SlateStyleRegistry.h"
#include "Styling/SlateTypes.h"
#include "Styling/CoreStyle.h"
#include "Interfaces/IPluginManager.h"
#include "SlateOptMacros.h"
#include "Styling/SlateStyle.h"


#define IMAGE_BRUSH( RelativePath, ... ) FSlateImageBrush( Style->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )
#define BOX_BRUSH( RelativePath, ... ) FSlateBoxBrush( Style->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )
#define BORDER_BRUSH( RelativePath, ... ) FSlateBorderBrush( Style->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )

#define DEFAULT_FONT(...) FCoreStyle::GetDefaultFontStyle(__VA_ARGS__)

FString FVoltEditorStyle::InContent(const FString& RelativePath, const ANSICHAR* Extension)
{
	static FString ContentDir = IPluginManager::Get().FindPlugin(TEXT("JointEditor"))->GetContentDir();
	return (ContentDir / RelativePath) + Extension;
}

TSharedPtr<ISlateStyle> FVoltEditorStyle::Instance = nullptr;


void FVoltEditorStyle::ResetToDefault() { SetStyle(FVoltEditorStyle::Create()); }

const FName FVoltEditorStyle::GetUEEditorSlateStyleSetName()
{
#if ENGINE_MAJOR_VERSION >= 5 && ENGINE_MINOR_VERSION >= 1
	return FAppStyle::GetAppStyleSetName();
#else
	return FEditorStyle::GetStyleSetName();
#endif
}

void FVoltEditorStyle::SetStyle(const TSharedRef<ISlateStyle>& NewStyle)
{
	if(Instance.IsValid()) { FSlateStyleRegistry::UnRegisterSlateStyle(*Instance.Get()); }

	Instance = NewStyle;

	if(Instance.IsValid()) { FSlateStyleRegistry::RegisterSlateStyle(*Instance.Get()); }
	else { ResetToDefault(); }
}


FName FVoltEditorStyle::GetStyleSetName()
{
	static FName VoltEditorStyleSetName(TEXT("VoltEditorStyle"));
	return VoltEditorStyleSetName;
}

const ISlateStyle& FVoltEditorStyle::GetUEEditorSlateStyleSet()
{
#if ENGINE_MAJOR_VERSION >= 5 && ENGINE_MINOR_VERSION >= 1
		return FAppStyle::Get();
#else
	return FEditorStyle::Get();
#endif
}

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

TSharedRef<ISlateStyle> FVoltEditorStyle::Create()
{
	// Const icon sizes
	const FVector2D Icon8x8(8.0f, 8.0f);
	const FVector2D Icon16x16(16.0f, 16.0f);
	const FVector2D Icon20x20(20.0f, 20.0f);

	const FVector2D Icon24x24(24.0f, 24.0f);
	const FVector2D Icon32x32(32.0f, 32.0f);
	const FVector2D Icon40x40(40.0f, 40.0f);
	const FVector2D Icon48x48(40.0f, 40.0f);

	const FVector2D Icon64x64(64.0f, 64.0f);
	const FVector2D Icon128x128(128.0f, 128.0f);

	const FMargin BoxMargin(0.20f);
	const FMargin TightMargin(0.4f);
	const FMargin FragmentMargin(0.4f);
	const FMargin CommandMargin(0.10f);


	const FMargin SphereBorderMargin(0.2f, 0.5f);
	const FMargin BorderMargin(0.5f);


	TSharedRef<FSlateStyleSet> Style = MakeShareable(new FSlateStyleSet("VoltEditorStyle"));
	Style->SetContentRoot(FPaths::EngineContentDir() / TEXT("Slate"));

	FString RootDirectory;

	auto ProjectPluginDir = FPaths::ConvertRelativePathToFull(
		FPaths::Combine(FPaths::ProjectPluginsDir() + "Joint"));
	auto ProjectPluginDir2 = FPaths::ConvertRelativePathToFull(
		FPaths::Combine(FPaths::ProjectPluginsDir() + "Joint"));
	auto EnginePluginDir = FPaths::ConvertRelativePathToFull(
		FPaths::Combine(FPaths::EnginePluginsDir() + TEXT("Marketplace/Joint")));

	// Is it in the project plugin folder?
	if(FPaths::DirectoryExists(ProjectPluginDir)) { RootDirectory = ProjectPluginDir; }
	// Is it in the project plugin folder with alternative name??
	else if(FPaths::DirectoryExists(ProjectPluginDir2)) { RootDirectory = ProjectPluginDir2; }
	// Is it in the engines marketplace folder ?
	else if(FPaths::DirectoryExists(EnginePluginDir)) { RootDirectory = EnginePluginDir; }
	

	{
		const float CornerRadius = 4.0f;
		const FVector4 AllCorners(CornerRadius, CornerRadius, CornerRadius, CornerRadius);
		const FVector4 LeftCorners(CornerRadius, 0.0f, 0.0f, CornerRadius);
		const FVector4 MiddleCorners(0.0f, 0.0f, 0.0f, 0.0f);
		const FVector4 RightCorners(0.0f, CornerRadius, CornerRadius, 0.0f);

		const float RounderCornerRadius = 12.0f;
		const FVector4 AllRounderCorners(RounderCornerRadius, RounderCornerRadius, RounderCornerRadius, RounderCornerRadius);
		const FVector4 LeftRounderCorners(RounderCornerRadius, 0.0f, 0.0f, RounderCornerRadius);
		const FVector4 MiddleRounderCorners(0.0f, 0.0f, 0.0f, 0.0f);
		const FVector4 RightRounderCorners(0.0f, RounderCornerRadius, RounderCornerRadius, 0.0f);

		Style->Set("VoltEditorStyle.RoundedBox.Test", new FSlateRoundedBoxBrush(FLinearColor(0.05,0.1,0.2), CornerRadius));
	}

	{ // TextBlockStyle

		const FTextBlockStyle RichTextNormal = FTextBlockStyle()
		.SetFont(DEFAULT_FONT("Regular", 10))
		.SetColorAndOpacity(FLinearColor(1, 1, 1, 1))
		.SetShadowOffset(FVector2D::ZeroVector)
		.SetShadowColorAndOpacity(FLinearColor::Black)
		.SetHighlightColor(FLinearColor(1.f, 1.f, 1.f));

		Style->Set("BetterTextSystemUI.Font.Default", RichTextNormal);

	}


	{ //ButtonStyle

		const FLinearColor NormalColor(0.01, 0.01, 0.01);
		const FLinearColor HoverColor(0.1, 0.1, 0.1);
		const FLinearColor SelectedColor(0.2, 0.2, 0.2);

		
		const FButtonStyle ToolbarTextPlayButton = FButtonStyle()
		.SetNormal(*GetUEEditorSlateStyleSet().GetBrush(TEXT("GraphEditor.Macro.DoOnce_16x")))
		.SetHovered(*GetUEEditorSlateStyleSet().GetBrush(TEXT("GraphEditor.Macro.DoOnce_16x")))
		.SetPressed(*GetUEEditorSlateStyleSet().GetBrush(TEXT("GraphEditor.Macro.DoOnce_16x")))
		.SetNormalPadding(FMargin(0, 0, 0, 0))
		.SetPressedPadding(FMargin(1, 1, 1, 1));

		Style->Set("BetterTextSystemUI.Button.ToolbarTextPlay", ToolbarTextPlayButton);

	}
	

	return Style;
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

#undef IMAGE_BRUSH
#undef BOX_BRUSH
#undef BORDER_BRUSH

#undef DEFAULT_FONT

