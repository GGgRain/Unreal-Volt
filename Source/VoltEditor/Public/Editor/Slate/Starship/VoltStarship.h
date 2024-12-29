#pragma once

#include "CoreMinimal.h"
#include "VoltAnimationManager.h"
#include "Widgets/SCompoundWidget.h"

class SVoltStarshipDescriptionCard;
class SVoltStarshipTooltip;
class SScrollBox;

class VOLTEDITOR_API SVoltStarship : public SCompoundWidget
{
	
public:

	SLATE_BEGIN_ARGS(SVoltStarship) {}
		SLATE_ARGUMENT(TSharedPtr<class FTabManager>, TabManager)
	SLATE_END_ARGS();
	
	void Construct(const FArguments& InArgs);

public:

	void FillWindowMenu(FMenuBuilder& MenuBuilder);
	
	TSharedRef<SWidget> MakeToolbar();

public:

	TSharedPtr<FTabManager> TabManager;
	
};


class VOLTEDITOR_API SVoltModuleStarshipTab : public SCompoundWidget
{
	
public:

	SLATE_BEGIN_ARGS(SVoltModuleStarshipTab) {}
	SLATE_END_ARGS();
	void Construct(const FArguments& InArgs);

public:

	TSharedRef<SWidget> CreateInterpSection_Rate();

	TSharedRef<SWidget> CreateInterpSection_Alpha();


public:

	UVoltAnimationManager* AnimationManager = nullptr;
	
};


class VOLTEDITOR_API SVoltSampleStarshipTab : public SCompoundWidget
{
	
public:

	SLATE_BEGIN_ARGS(SVoltSampleStarshipTab) {}
	SLATE_END_ARGS();

	void Construct(const FArguments& InArgs);

public:
	
	TSharedRef<SWidget> CreateButtonSection();

	TSharedRef<SWidget> CreateExpandingBorderSection();

	void ResetExpandingBorderSection();

	TSharedRef<SWidget> CreateScrollSection();

	TSharedPtr<SScrollBox> ScrollBox;

	void ResetScrollSection();

	TSharedRef<SWidget> CreateDescriptionSection();

	void ResetCard();

	void Callback_Tooltip_Reset(TSharedRef<SVoltStarshipTooltip> Tooltip);

public:

	UVoltAnimationManager* AnimationManager = nullptr;

public:

	TSharedPtr<IToolTip> TooltipWidget;

	TSharedPtr<SBox> DescriptionCardBox;

	TSharedPtr<SBox> ExpandingBorderBox;

	
public:

	TSharedPtr<SVoltStarshipDescriptionCard> TooltipDescriptionCard;

};
