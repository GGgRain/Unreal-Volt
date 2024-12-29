#pragma once

#include "CoreMinimal.h"
#include "VoltAnimationManager.h"
#include "Slate/WidgetTransform.h"
#include "Widgets/SCompoundWidget.h"


struct FWidgetTransform;

class VOLTEDITOR_API SVoltInterpModuleBackground : public SCompoundWidget
{
	
public:

	SLATE_BEGIN_ARGS(SVoltInterpModuleBackground) :
		_AnimationManager(nullptr)
		{}
	SLATE_DEFAULT_SLOT(FArguments, Content)
	SLATE_ARGUMENT(FText, Title)
	SLATE_ARGUMENT(UVoltAnimationManager*, AnimationManager)
	SLATE_ARGUMENT(UVoltAnimation*, AnimationToPlay)
	SLATE_END_ARGS();
	
	void Construct(const FArguments& InArgs);

public:
	
	TSharedPtr<SBorder> AnimBlock;

	FText Title;

public:

	UVoltAnimationManager* AnimationManager = nullptr;

	UVoltAnimation* Animation = nullptr;
	
};



class VOLTEDITOR_API SVoltStarshipBorder : public SCompoundWidget
{
	
public:

	SLATE_BEGIN_ARGS(SVoltStarshipBorder) :
		_AnimationManager(nullptr),
		_NormalOpacity(1),
		_HoverOpacity(1),
		_NormalColor(FLinearColor(0.01,0.01,0.01)),
		_HoverColor(FLinearColor(0.02,0.02,0.02)),
		_OutlineNormalColor(FLinearColor(0.01,0.01,0.01)),
		_OutlineHoverColor(FLinearColor(0.1,0.1,0.1)),
		_NormalTransform(FWidgetTransform(FVector2D(0, 0), FVector2D(1,1), FVector2D(0, 0), 0)),
		_HoverTransform(FWidgetTransform(FVector2D(0, 0), FVector2D(1, 1), FVector2D(0, 0), 0)),
		_ContentMargin(FMargin(10)),
		_OutlineMargin(FMargin(1))
		{}

		SLATE_ARGUMENT( EHorizontalAlignment, HAlign )
		SLATE_ARGUMENT( EVerticalAlignment, VAlign )
		
		SLATE_DEFAULT_SLOT(FArguments, Content)
		SLATE_ARGUMENT(UVoltAnimationManager*, AnimationManager)
			
		SLATE_ATTRIBUTE( const FSlateBrush*, OuterBorderImage )
		SLATE_ATTRIBUTE( const FSlateBrush*, InnerBorderImage )
		
		SLATE_ARGUMENT(float, NormalOpacity )
		SLATE_ARGUMENT(float, HoverOpacity )
			
		SLATE_ARGUMENT(FLinearColor, NormalColor)
		SLATE_ARGUMENT(FLinearColor, HoverColor)

		SLATE_ARGUMENT(FLinearColor, OutlineNormalColor)
		SLATE_ARGUMENT(FLinearColor, OutlineHoverColor)

		SLATE_ARGUMENT( FWidgetTransform, NormalTransform )
		SLATE_ARGUMENT( FWidgetTransform, HoverTransform )

		SLATE_ARGUMENT(FMargin, ContentMargin)
		SLATE_ARGUMENT(FMargin, OutlineMargin)
		
	SLATE_END_ARGS();
	
	void Construct(const FArguments& InArgs);

public:
	
	FLinearColor NormalColor;
	FLinearColor HoverColor;
	FLinearColor OutlineNormalColor;
	FLinearColor OutlineHoverColor;

	float NormalOpacity;
	float HoverOpacity;
public:

	FWidgetTransform NormalTransform;
	FWidgetTransform HoverTransform;
	
public:

	TSharedPtr<SWidget> Content;

	TSharedPtr<SWidget> OuterBorder;
	
	TSharedPtr<SWidget> InnerBorder;


public:

	UVoltAnimationManager* AnimationManager = nullptr;
	
public:

	
	virtual void OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	
	virtual void OnMouseLeave(const FPointerEvent& MouseEvent) override;
	
};



class VOLTEDITOR_API SVoltStarshipButton : public SCompoundWidget
{
	
public:

	SLATE_BEGIN_ARGS(SVoltStarshipButton) :
		_AnimationManager(nullptr),
		_ButtonStyle( &FCoreStyle::Get().GetWidgetStyle< FButtonStyle >( "Button" ) ),
		_NormalColor(FLinearColor(0.02,0.02,0.02)),
		_HoverColor(FLinearColor(0.04,0.04,0.08)),
		_PressColor(FLinearColor(0.06,0.06,0.12)),
		
		_OutlineNormalColor(FLinearColor(0.02,0.02,0.02)),
		_OutlineHoverColor(FLinearColor(0.2,0.2,0.4)),
		_OutlinePressColor(FLinearColor(0.5,0.5,0.5)),

		_ContentMargin(FMargin(10)),
		_OutlineMargin(FMargin(1))
		{}
		SLATE_DEFAULT_SLOT(FArguments, Content)

		SLATE_ARGUMENT( EHorizontalAlignment, HAlign )
		SLATE_ARGUMENT( EVerticalAlignment, VAlign )
		
		SLATE_ARGUMENT(UVoltAnimationManager*, AnimationManager)

		SLATE_ATTRIBUTE( const FSlateBrush*, OutlineBorderImage )
		SLATE_STYLE_ARGUMENT(FButtonStyle, ButtonStyle)
		
		SLATE_ARGUMENT(FLinearColor, NormalColor)
		SLATE_ARGUMENT(FLinearColor, HoverColor)
		SLATE_ARGUMENT(FLinearColor, PressColor)

		SLATE_ARGUMENT(FLinearColor, OutlineNormalColor)
		SLATE_ARGUMENT(FLinearColor, OutlineHoverColor)
		SLATE_ARGUMENT(FLinearColor, OutlinePressColor)

		SLATE_ARGUMENT(FMargin, ContentMargin)
		SLATE_ARGUMENT(FMargin, OutlineMargin)


		/** Called when the button is clicked */
		SLATE_EVENT( FOnClicked, OnClicked )
		/** Called when the button is pressed */
		SLATE_EVENT( FSimpleDelegate, OnPressed )
		/** Called when the button is released */
		SLATE_EVENT( FSimpleDelegate, OnReleased )
		SLATE_EVENT( FSimpleDelegate, OnHovered )
		SLATE_EVENT( FSimpleDelegate, OnUnhovered )

	SLATE_END_ARGS();
	
	void Construct(const FArguments& InArgs);


public:
	
	FLinearColor NormalColor;
	FLinearColor HoverColor;
	FLinearColor PressColor;
	
	FLinearColor OutlineNormalColor;
	FLinearColor OutlineHoverColor;
	FLinearColor OutlinePressColor;
	
public:

	TSharedPtr<SButton> Button;
	TSharedPtr<SBorder> Border;
	TSharedPtr<SWidget> Content;

public:

	FOnClicked OnClicked;

	/** The delegate to execute when the button is pressed */
	FSimpleDelegate OnPressed;

	/** The delegate to execute when the button is released */
	FSimpleDelegate OnReleased;

	/** The delegate to execute when the button is hovered */
	FSimpleDelegate OnHovered;

	/** The delegate to execute when the button exit the hovered state */
	FSimpleDelegate OnUnhovered;

public:
	
	UVoltAnimationManager* AnimationManager = nullptr;

public:

	FVoltAnimationTrack OutlineColorTrack;
	FVoltAnimationTrack ColorTrack;
	FVoltAnimationTrack ContentColorTrack;
	FVoltAnimationTrack TransformTrack;

public:

	void EventOnPressed();
	void EventOnHovered();
	void EventOnUnhovered();
	
};


class VOLTEDITOR_API SVoltStarshipToggleButton : public SCompoundWidget
{

public:
	
	SLATE_BEGIN_ARGS(SVoltStarshipToggleButton) :
		_AnimationManager(nullptr),
		_ButtonStyle( &FCoreStyle::Get().GetWidgetStyle< FButtonStyle >( "Button" )),
		_OnColor(FLinearColor(0.1,0.4,0.2)),
		_OffColor(FLinearColor(0.05,0.05,0.05)),
		_ButtonLength(25),
		_ToggleSize(10)
	{}
	SLATE_ARGUMENT(UVoltAnimationManager*, AnimationManager)
	SLATE_ATTRIBUTE( const FSlateBrush*, ToggleImage )
	SLATE_STYLE_ARGUMENT(FButtonStyle, ButtonStyle)
	SLATE_ARGUMENT(FLinearColor, OnColor)
	SLATE_ARGUMENT(FLinearColor, OffColor)
	SLATE_ARGUMENT(float, ButtonLength)
	SLATE_ARGUMENT(float, ToggleSize)

		
	SLATE_EVENT( FSimpleDelegate, OnPressed )
	SLATE_EVENT( FSimpleDelegate, OnHovered )
	SLATE_EVENT( FSimpleDelegate, OnUnhovered )
	SLATE_END_ARGS();

public:
	
	void Construct(const FArguments& InArgs);

public:

	UVoltAnimationManager* AnimationManager;

public:

	TSharedPtr<SButton> Button;
	
	TSharedPtr<SWidget> Toggle;

public:

	/** The delegate to execute when the button is pressed */
	FSimpleDelegate OnPressed;

	/** The delegate to execute when the button is hovered */
	FSimpleDelegate OnHovered;

	/** The delegate to execute when the button exit the hovered state */
	FSimpleDelegate OnUnhovered;

public:

	FLinearColor OnColor;
	FLinearColor OffColor;
	
public:

	bool bIsToggled = false;

	float ButtonLength = 0;

public:

	void PlayToggleAnimation(const bool& bToggled, const bool& bInstant);
	
public:

	void EventOnPressed();
	void EventOnHovered();
	void EventOnUnhovered();
	
};




class VOLTEDITOR_API SVoltStarshipScrollElement : public SCompoundWidget
{
	
public:

	SLATE_BEGIN_ARGS(SVoltStarshipScrollElement) :
		_AnimationManager(nullptr)
		{}
	SLATE_DEFAULT_SLOT(FArguments, Content)
	SLATE_ARGUMENT(UVoltAnimationManager*, AnimationManager)
	SLATE_ATTRIBUTE( const FSlateBrush*, OutlineBorderImage )
	SLATE_ARGUMENT(float, Interval)

		
	SLATE_END_ARGS();
	
	void Construct(const FArguments& InArgs);
	
public:

	TSharedPtr<SBorder> Border;

	TSharedPtr<SWidget> Content;

public:
	
	UVoltAnimationManager* AnimationManager = nullptr;

public:

	FVoltAnimationTrack TransformTrack;

public:

	void PlayAnimation(const double& Interval);
	
};




class VOLTEDITOR_API SVoltStarshipExpandingBorder : public SCompoundWidget
{
	
public:

	SLATE_BEGIN_ARGS(SVoltStarshipExpandingBorder) :
		_AnimationManager(nullptr)
	{}
	SLATE_DEFAULT_SLOT(FArguments, Content)
	SLATE_ARGUMENT(UVoltAnimationManager*, AnimationManager)
	SLATE_ARGUMENT(float, DelayDuration)

	SLATE_END_ARGS();
	
	void Construct(const FArguments& InArgs);
	
public:

	TSharedPtr<SBox> Box;

	TSharedPtr<SWidget> Content;

public:
	
	UVoltAnimationManager* AnimationManager = nullptr;

public:

	float DelayDuration;

public:
	
	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;
	
public:

	void PlayAnimation();

private:

	bool bEverPlayedAnimation = false;
	
};










class VOLTEDITOR_API SVoltStarshipDescriptionCard : public SCompoundWidget
{
	
public:

	SLATE_BEGIN_ARGS(SVoltStarshipDescriptionCard) :
		_AnimationManager(nullptr),
		_Size(FVector2D(800,600)),
		_ExplanationSectionColor(FLinearColor(0.5,0.6,0.8)),
		_ExplanationBackgroundColor(FLinearColor(0.01,0.01,0.01))
	{}
	SLATE_DEFAULT_SLOT(FArguments, Content)
	SLATE_ARGUMENT(UVoltAnimationManager*, AnimationManager)
		
	SLATE_ARGUMENT(FText, Title)
	SLATE_ARGUMENT(FText, Description)
	SLATE_ARGUMENT(FVector2D, Size)
		
	SLATE_ARGUMENT(FLinearColor, ExplanationSectionColor)
	SLATE_ARGUMENT(FLinearColor, ExplanationBackgroundColor)

	SLATE_END_ARGS();

public:
	
	void PlayInitAnimation();
	
	void Construct(const FArguments& InArgs);
	
public:

	TSharedPtr<SBorder> Border;

	TSharedPtr<SBorder> DescriptionBorder;

	TSharedPtr<STextBlock> TitleTextBlock;
	TSharedPtr<STextBlock> DescriptionTextBlock;

	TSharedPtr<SWidget> Content;

public:
	
	UVoltAnimationManager* AnimationManager = nullptr;

public:

	FVoltAnimationTrack TransformTrack;
	
};









class VOLTEDITOR_API SVoltStarshipTooltip
	: public SCompoundWidget
	, public IToolTip
{
public:
	DECLARE_DELEGATE_OneParam(FOnSetInteractiveWindowLocation, FVector2D&);

	DECLARE_DELEGATE_OneParam(FOnVoltStarshipTooltipOpened, TSharedRef<SVoltStarshipTooltip>);


	SLATE_BEGIN_ARGS( SVoltStarshipTooltip )
		: _Text()
		, _Content()
		, _Font(FCoreStyle::Get().GetFontStyle("ToolTip.Font"))
		, _TextMargin(FMargin(8.0f))
		, _BorderImage(FCoreStyle::Get().GetBrush("ToolTip.Background"))
		, _IsInteractive(false)
		, _OnSetInteractiveWindowLocation()
		, _OnVoltStarshipTooltipOpened()

	{ }

		/** The text displayed in this tool tip */
		SLATE_ATTRIBUTE(FText, Text)
		/** Arbitrary content to be displayed in the tool tip; overrides any text that may be set. */
		SLATE_DEFAULT_SLOT(FArguments, Content)
		/** The font to use for this tool tip */
		SLATE_ATTRIBUTE(FSlateFontInfo, Font)
		/** Margin between the tool tip border and the text content */
		SLATE_ATTRIBUTE(FMargin, TextMargin)
		/** The background/border image to display */
		SLATE_ATTRIBUTE(const FSlateBrush*, BorderImage)
		/** Whether the tooltip should be considered interactive */
		SLATE_ATTRIBUTE(bool, IsInteractive)
		/** Hook to modify or override the desired location (in screen space) for interactive tooltip windows. By default, the previous frame's cursor position will be used. */
		SLATE_EVENT(FOnSetInteractiveWindowLocation, OnSetInteractiveWindowLocation)
		SLATE_EVENT(FOnVoltStarshipTooltipOpened, OnVoltStarshipTooltipOpened)

	SLATE_END_ARGS()

	/**
	 * Construct this widget
	 *
	 * @param	InArgs	The declaration data for this widget
	 */
	void Construct( const FArguments& InArgs );

public:

	// IToolTip interface

	virtual TSharedRef<class SWidget> AsWidget( ) override
	{
		return AsShared();
	}

	virtual TSharedRef<SWidget> GetContentWidget( ) override
	{
		return ToolTipContent.ToSharedRef();
	}

	virtual void SetContentWidget(const TSharedRef<SWidget>& InContentWidget) override;
	virtual void ResetContentWidget () override;

	virtual bool IsEmpty( ) const override;
	virtual bool IsInteractive( ) const override;
	virtual void OnOpening() override;
	virtual void OnClosed() override;
	virtual void OnSetInteractiveWindowLocation(FVector2D& InOutDesiredLocation) const override;

	virtual const FText& GetTextTooltip() const
	{
		return TextContent.Get();
	}

public:

	static float GetToolTipWrapWidth();

private:

	FOnVoltStarshipTooltipOpened OnVoltStarshipTooltipOpened;

private:

	// Text block widget.
	TAttribute<FText> TextContent;

	// Content widget.
	TWeakPtr<SWidget> WidgetContent;

	// Wrapped content within the widget;
	TSharedPtr<SWidget> ToolTipContent;

	// Font used for the text displayed (where applicable)
	TAttribute<FSlateFontInfo> Font;

	// Margin between the tool tip border and the text content
	TAttribute<FMargin> TextMargin;

	// The background/border image to display
	TAttribute<const FSlateBrush*> BorderImage;
	
	// Whether the tooltip should be considered interactive.
	TAttribute<bool> bIsInteractive;

	// Optional delegate to modify or override the desired location for an interactive tooltip.
	FOnSetInteractiveWindowLocation OnSetInteractiveWindowLocationDelegate;
};


