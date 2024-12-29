//Copyright 2022~2024 DevGrain. All Rights Reserved.

#include "VoltEditorStyle.h"

#include "Styling/SlateStyleRegistry.h"
#include "Styling/SlateTypes.h"
#include "Styling/CoreStyle.h"
#include "SlateOptMacros.h"
#include "Styling/SlateStyle.h"

TSharedPtr<ISlateStyle> FVoltEditorStyle::Instance = nullptr;


void FVoltEditorStyle::ResetToDefault() { SetStyleSet(FVoltEditorStyle::Create()); }

const ISlateStyle& FVoltEditorStyle::GetUEEditorSlateStyleSet()
{
#if UE_VERSION_OLDER_THAN(5,0,0)
	return FEditorStyle::Get();
#else
	return FAppStyle::Get();
#endif
}

const FName FVoltEditorStyle::GetUEEditorSlateStyleSetName()
{

#if UE_VERSION_OLDER_THAN(5,0,0)
	return FEditorStyle::GetStyleSetName();
#else
	return FAppStyle::GetAppStyleSetName();
#endif
}

void FVoltEditorStyle::SetStyleSet(const TSharedRef<ISlateStyle>& NewStyle)
{
	if (Instance.IsValid())
	{
		FSlateStyleRegistry::UnRegisterSlateStyle(*Instance.Get());
	}
	else if (const ISlateStyle* StyleSet = FSlateStyleRegistry::FindSlateStyle(GetStyleSetName()))
	{
		FSlateStyleRegistry::UnRegisterSlateStyle(*StyleSet);
	}
	
	Instance = NewStyle;

	if (Instance.IsValid()) { FSlateStyleRegistry::RegisterSlateStyle(*Instance.Get()); }
	else { ResetToDefault(); }
}


FName FVoltEditorStyle::GetStyleSetName()
{
	return TEXT("VoltStyle");
}


#define VOLT_IMAGE_BRUSH( RelativePath, ... ) FSlateImageBrush( Style->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )
#define VOLT_BOX_BRUSH( RelativePath, ... ) FSlateBoxBrush( Style->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )
#define VOLT_BORDER_BRUSH( RelativePath, ... ) FSlateBorderBrush( Style->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )

#define VOLT_DEFAULT_FONT(...) FCoreStyle::GetDefaultFontStyle(__VA_ARGS__)

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

TSharedRef<ISlateStyle> FVoltEditorStyle::Create()
{
	TSharedRef<FSlateStyleSet> Style = MakeShareable(new FSlateStyleSet(GetStyleSetName()));


	Style->Set("VoltStarship.Image.Box", new FSlateRoundedBoxBrush(FLinearColor::White, 0.01));

	Style->Set("VoltStarship.Image.Sphere", new FSlateRoundedBoxBrush(FLinearColor::White));

	Style->Set("VoltStarship.Image.Transparent", new FSlateRoundedBoxBrush(FLinearColor::Transparent, 0.01));



	//Border
	Style->Set("VoltStarship.Border.Round", new FSlateRoundedBoxBrush(FLinearColor::White, 4.f));

	Style->Set("VoltStarship.Border.Rounder", new FSlateRoundedBoxBrush(FLinearColor::White, 11.f));
	
	Style->Set("VoltStarship.Border.Sphere", new FSlateRoundedBoxBrush(FLinearColor::White));

	
	//Fonts
	const FTextBlockStyle FontWhite = FTextBlockStyle()
		.SetFont(VOLT_DEFAULT_FONT("Regular", 12))
		.SetColorAndOpacity(FSlateColor::UseForeground())
		.SetShadowOffset(FVector2D::ZeroVector)
		.SetShadowColorAndOpacity(FLinearColor::Black);

	Style->Set("VoltStarship.Font.White", FontWhite);

	const FTextBlockStyle FontWhiteBig = FTextBlockStyle()
		.SetFont(VOLT_DEFAULT_FONT("Regular", 15))
		.SetColorAndOpacity(FSlateColor::UseForeground())
		.SetShadowOffset(FVector2D::ZeroVector)
		.SetShadowColorAndOpacity(FLinearColor::Black);

	Style->Set("VoltStarship.Font.White.Big", FontWhiteBig);

	const FTextBlockStyle FontWhiteBiggest = FTextBlockStyle()
	.SetFont(VOLT_DEFAULT_FONT("Regular", 48))
	.SetColorAndOpacity(FSlateColor::UseForeground())
	.SetShadowOffset(FVector2D::ZeroVector)
	.SetShadowColorAndOpacity(FLinearColor::Black);

	Style->Set("VoltStarship.Font.White.Biggest", FontWhiteBiggest);

	
	//Buttons

	const FButtonStyle RoundButton = FButtonStyle()
	.SetNormal(FSlateRoundedBoxBrush(FLinearColor::White, 4))
	.SetHovered(FSlateRoundedBoxBrush(FLinearColor::White, 4))
	.SetPressed(FSlateRoundedBoxBrush(FLinearColor::White, 4))
	.SetNormalPadding(FMargin(0))
	.SetPressedPadding(FMargin(0));
	
	Style->Set("VoltStarship.Button.Round", RoundButton);


	const FButtonStyle RounderButton = FButtonStyle()
	.SetNormal(FSlateRoundedBoxBrush(FLinearColor::White, 10))
	.SetHovered(FSlateRoundedBoxBrush(FLinearColor::White, 10))
	.SetPressed(FSlateRoundedBoxBrush(FLinearColor::White, 10))
	.SetNormalPadding(FMargin(0))
	.SetPressedPadding(FMargin(0));
	
	Style->Set("VoltStarship.Button.Rounder", RounderButton);


	const FButtonStyle SphereButton = FButtonStyle()
	.SetNormal(FSlateRoundedBoxBrush(FLinearColor::White))
	.SetHovered(FSlateRoundedBoxBrush(FLinearColor::White))
	.SetPressed(FSlateRoundedBoxBrush(FLinearColor::White))
	.SetNormalPadding(FMargin(0))
	.SetPressedPadding(FMargin(0));
	
	Style->Set("VoltStarship.Button.Sphere", SphereButton);

	return Style;
}

const ISlateStyle& FVoltEditorStyle::Get()
{
	return *(Instance.Get());
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

#undef VOLT_IMAGE_BRUSH
#undef VOLT_BOX_BRUSH
#undef VOLT_BORDER_BRUSH

#undef VOLT_DEFAULT_FONT
