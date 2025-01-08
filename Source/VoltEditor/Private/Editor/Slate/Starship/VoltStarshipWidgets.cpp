#include <Starship/VoltStarshipWidgets.h>

#include "VoltDecl.h"
#include "VoltEditorStyle.h"
#include "Module/Volt_ASM_Delay.h"
#include "Module/Volt_ASM_InterpBackgroundColor.h"
#include "Module/Volt_ASM_InterpBoxProperties.h"
#include "Module/Volt_ASM_InterpColor.h"
#include "Module/Volt_ASM_InterpForegroundColor.h"
#include "Module/Volt_ASM_InterpRenderOpacity.h"
#include "Module/Volt_ASM_InterpWidgetTransform.h"
#include "Module/Volt_ASM_Sequence.h"
#include "Module/Volt_ASM_SetWidgetTransformPivot.h"
#include "Module/Volt_ASM_Simultaneous.h"


#define LOCTEXT_NAMESPACE "VoltStarship"

#define VOLT_STARSHIP_BORDER_MARGIN 4
#define VOLT_STARSHIP_MARGIN_GAP 2

#define VOLT_STARSHIP_BORDER_BACKGROUND_COLOR FLinearColor(0.01,0.01,0.01)
#define VOLT_STARSHIP_BORDER_BACKGROUND_COLOR_BRIGHT FLinearColor(0.03,0.03,0.03)
#define VOLT_STARSHIP_BORDER_BACKGROUND_COLOR_BRIGHT_HOVER FLinearColor(0.30,0.3,0.3)

void SVoltInterpModuleBackground::Construct(const FArguments& InArgs)
{

	Animation = InArgs._AnimationToPlay;

	Title = InArgs._Title;

	ChildSlot[
		SNew(SOverlay)
		+ SOverlay::Slot()
		[
			SNew(SSpacer)
			.Size(FVector2D(200, 100))
		]
		+ SOverlay::Slot()
		[
			SNew(SVoltStarshipBorder)
			.ContentMargin(FMargin(10))
			.OutlineMargin(FMargin(1))
			.InnerBorderImage(FVoltEditorStyle::Get().GetBrush("VoltStarship.Border.Round"))
			.OuterBorderImage(FVoltEditorStyle::Get().GetBrush("VoltStarship.Border.Round"))
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.Padding(VOLT_STARSHIP_BORDER_MARGIN)
				.FillHeight(1)
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				[
					SNew(STextBlock)
					.Justification(ETextJustify::Center)
					.Text(Title)
				]
				+ SVerticalBox::Slot()
				.Padding(VOLT_STARSHIP_BORDER_MARGIN)
				.FillHeight(1)
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				[
					SAssignNew(AnimBlock, SBorder)
					.BorderImage(FVoltEditorStyle::Get().GetBrush("VoltStarship.Border.Round"))
					.Padding(VOLT_STARSHIP_BORDER_MARGIN * 2)
				]
			]
		]
	];

	//Get AsShared() to wait until it settle down.
	VOLT_PLAY_ANIM(AnimBlock->AsShared(), Animation);
}


void SVoltStarshipBorder::Construct(const FArguments& InArgs)
{

	Content = InArgs._Content.Widget;

	NormalColor = InArgs._NormalColor;
	HoverColor = InArgs._HoverColor;

	OutlineNormalColor = InArgs._OutlineNormalColor;
	OutlineHoverColor = InArgs._OutlineHoverColor;

	NormalTransform = InArgs._NormalTransform;
	HoverTransform = InArgs._HoverTransform;

	NormalOpacity = InArgs._NormalOpacity;
	HoverOpacity = InArgs._HoverOpacity;

	ChildSlot[
		SAssignNew(OuterBorder, SBorder)
		.RenderTransformPivot(FVector2D(0.5, 0.5))
		.RenderOpacity(NormalOpacity)
		.BorderBackgroundColor(OutlineNormalColor)
		.BorderImage(InArgs._OuterBorderImage)
		.Padding(InArgs._OutlineMargin)
		.VAlign(VAlign_Fill)
		.HAlign(HAlign_Fill)
		[
			SAssignNew(InnerBorder, SBorder)
			.BorderBackgroundColor(NormalColor)
			.BorderImage(InArgs._InnerBorderImage)
			.Padding(InArgs._ContentMargin)
			.VAlign(InArgs._VAlign)
			.HAlign(InArgs._HAlign)
			[
				InArgs
				._Content
				.Widget
			]
		]
	];
}

void SVoltStarshipBorder::OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	VOLT_STOP_ALL_ANIM(InnerBorder->AsShared());

	VOLT_STOP_ALL_ANIM(OuterBorder->AsShared());

	UVoltAnimation* ContentAnim = VOLT_MAKE_ANIMATION()(
		VOLT_MAKE_MODULE(UVolt_ASM_InterpBackgroundColor)
		.TargetColor(HoverColor)
		.RateBasedInterpSpeed(20)
	);

	UVoltAnimation* OutlineAnim = VOLT_MAKE_ANIMATION()(
		VOLT_MAKE_MODULE(UVolt_ASM_InterpBackgroundColor)
		.TargetColor(OutlineHoverColor)
		.RateBasedInterpSpeed(20)
	);

	UVoltAnimation* OutlineTransformAnim = VOLT_MAKE_ANIMATION()(
		VOLT_MAKE_MODULE(UVolt_ASM_InterpWidgetTransform)
		.TargetWidgetTransform(HoverTransform)
		.RateBasedInterpSpeed(20)
	);

	UVoltAnimation* OpacityAnim = VOLT_MAKE_ANIMATION()(
		VOLT_MAKE_MODULE(UVolt_ASM_InterpRenderOpacity)
		.TargetOpacity(HoverOpacity)
		.RateBasedInterpSpeed(20)
	);

	VOLT_PLAY_ANIM(InnerBorder->AsShared(), ContentAnim);

	VOLT_PLAY_ANIM(OuterBorder->AsShared(), OutlineAnim);

	VOLT_PLAY_ANIM(OuterBorder->AsShared(), OutlineTransformAnim);

	VOLT_PLAY_ANIM(OuterBorder->AsShared(), OpacityAnim);

	SCompoundWidget::OnMouseEnter(MyGeometry, MouseEvent);
}

void SVoltStarshipBorder::OnMouseLeave(const FPointerEvent& MouseEvent)
{
	VOLT_STOP_ALL_ANIM(InnerBorder->AsShared());

	VOLT_STOP_ALL_ANIM(OuterBorder->AsShared());

	UVoltAnimation* ContentAnim = VOLT_MAKE_ANIMATION()(
		VOLT_MAKE_MODULE(UVolt_ASM_InterpBackgroundColor)
		.TargetColor(NormalColor)
		.RateBasedInterpSpeed(20)
	);

	UVoltAnimation* OutlineAnim = VOLT_MAKE_ANIMATION()(
		VOLT_MAKE_MODULE(UVolt_ASM_InterpBackgroundColor)
		.TargetColor(OutlineNormalColor)
		.RateBasedInterpSpeed(20)
	);

	UVoltAnimation* OutlineTransformAnim = VOLT_MAKE_ANIMATION()(
		VOLT_MAKE_MODULE(UVolt_ASM_InterpWidgetTransform)
		.TargetWidgetTransform(NormalTransform)
		.RateBasedInterpSpeed(20)
	);

	UVoltAnimation* OpacityAnim = VOLT_MAKE_ANIMATION()(
		VOLT_MAKE_MODULE(UVolt_ASM_InterpRenderOpacity)
		.TargetOpacity(NormalOpacity)
		.RateBasedInterpSpeed(20)
	);

	VOLT_PLAY_ANIM(InnerBorder->AsShared(), ContentAnim);

	VOLT_PLAY_ANIM(OuterBorder->AsShared(), OutlineAnim);

	VOLT_PLAY_ANIM(OuterBorder->AsShared(), OutlineTransformAnim);

	VOLT_PLAY_ANIM(OuterBorder->AsShared(), OpacityAnim);

	SCompoundWidget::OnMouseLeave(MouseEvent);
}


void SVoltStarshipButton::Construct(const FArguments& InArgs)
{
	Content = InArgs._Content.Widget;
	
	NormalColor = InArgs._NormalColor;
	HoverColor = InArgs._HoverColor;
	PressColor = InArgs._PressColor;

	OutlineNormalColor = InArgs._OutlineNormalColor;
	OutlineHoverColor = InArgs._OutlineHoverColor;
	OutlinePressColor = InArgs._OutlinePressColor;

	OnClicked = InArgs._OnClicked;
	OnPressed = InArgs._OnPressed;
	OnReleased = InArgs._OnReleased;
	OnHovered = InArgs._OnHovered;
	OnUnhovered = InArgs._OnUnhovered;


	ChildSlot[
		SAssignNew(Border, SBorder)
		.BorderBackgroundColor(OutlineNormalColor)
		.BorderImage(InArgs._OutlineBorderImage)
		.Padding(InArgs._OutlineMargin)
		.VAlign(VAlign_Fill)
		.HAlign(HAlign_Fill)
		[
			SAssignNew(Button, SButton)
			.ButtonStyle(InArgs._ButtonStyle)
			.ButtonColorAndOpacity(NormalColor)
			.ContentPadding(InArgs._ContentMargin)
			.OnClicked(OnClicked)
			.OnReleased(OnReleased)
			.OnPressed(this, &SVoltStarshipButton::EventOnPressed)
			.OnHovered(this, &SVoltStarshipButton::EventOnHovered)
			.OnUnhovered(this, &SVoltStarshipButton::EventOnUnhovered)
			.VAlign(InArgs._VAlign)
			.HAlign(InArgs._HAlign)
			[
				Content.ToSharedRef()
			]
		]
	];

	EventOnUnhovered();
}

void SVoltStarshipButton::EventOnPressed()
{
	VOLT_STOP_ANIM(TransformTrack);
	VOLT_STOP_ANIM(ColorTrack);
	VOLT_STOP_ANIM(OutlineColorTrack);

	UVoltAnimation* Anim = VOLT_MAKE_ANIMATION()
	(
		VOLT_MAKE_MODULE(UVolt_ASM_SetWidgetTransformPivot)
		.TargetWidgetTransformPivot(FVector2D(0.5, 0.5)),
		VOLT_MAKE_MODULE(UVolt_ASM_Sequence)
		.bShouldLoop(false)
		(
			VOLT_MAKE_MODULE(UVolt_ASM_InterpWidgetTransform)
			.TargetWidgetTransform(FWidgetTransform(FVector2D(0, 0), FVector2D(0.95, 0.95), FVector2D(0, 0), 0))
			.RateBasedInterpSpeed(45),
			VOLT_MAKE_MODULE(UVolt_ASM_InterpWidgetTransform)
			.TargetWidgetTransform(FWidgetTransform(FVector2D(0, 0), FVector2D(1, 1), FVector2D(0, 0), 0))
			.RateBasedInterpSpeed(20)
		)
	);

	UVoltAnimation* ColorAnim = VOLT_MAKE_ANIMATION()
	(
		VOLT_MAKE_MODULE(UVolt_ASM_Sequence)
		.bShouldLoop(false)
		(
			VOLT_MAKE_MODULE(UVolt_ASM_InterpBackgroundColor)
			.TargetColor(PressColor)
			.RateBasedInterpSpeed(45),
			VOLT_MAKE_MODULE(UVolt_ASM_InterpBackgroundColor)
			.TargetColor(HoverColor)
			.RateBasedInterpSpeed(20)
		)
	);

	UVoltAnimation* BorderColorAnim = VOLT_MAKE_ANIMATION()
	(
		VOLT_MAKE_MODULE(UVolt_ASM_Sequence)
		.bShouldLoop(false)
		(
			VOLT_MAKE_MODULE(UVolt_ASM_InterpBackgroundColor)
			.TargetColor(OutlinePressColor)
			.RateBasedInterpSpeed(45),
			VOLT_MAKE_MODULE(UVolt_ASM_InterpBackgroundColor)
			.TargetColor(OutlineHoverColor)
			.RateBasedInterpSpeed(20)
		)
	);

	TransformTrack = VOLT_PLAY_ANIM(Button, Anim);

	ColorTrack = VOLT_PLAY_ANIM(Button, ColorAnim);

	OutlineColorTrack = VOLT_PLAY_ANIM(Border, BorderColorAnim);

	OnPressed.ExecuteIfBound();
}

void SVoltStarshipButton::EventOnHovered()
{
	VOLT_STOP_ANIM(ColorTrack);
	VOLT_STOP_ANIM(OutlineColorTrack);

	UVoltAnimation* Anim = VOLT_MAKE_ANIMATION()(
		VOLT_MAKE_MODULE(UVolt_ASM_InterpBackgroundColor)
		.TargetColor(HoverColor)
		.RateBasedInterpSpeed(20)
	);

	UVoltAnimation* BorderColorAnim = VOLT_MAKE_ANIMATION()
	(
		VOLT_MAKE_MODULE(UVolt_ASM_InterpBackgroundColor)
		.TargetColor(OutlineHoverColor)
		.RateBasedInterpSpeed(45)
	);
	
	ColorTrack = VOLT_PLAY_ANIM(Button, Anim);
	OutlineColorTrack = VOLT_PLAY_ANIM(Border, BorderColorAnim);

	OnHovered.ExecuteIfBound();
}

void SVoltStarshipButton::EventOnUnhovered()
{
	VOLT_STOP_ANIM(ColorTrack);
	VOLT_STOP_ANIM(OutlineColorTrack);
	
	UVoltAnimation* Anim = VOLT_MAKE_ANIMATION()(
		VOLT_MAKE_MODULE(UVolt_ASM_InterpBackgroundColor)
		.TargetColor(NormalColor)
		.RateBasedInterpSpeed(20)
	);

	UVoltAnimation* BorderColorAnim = VOLT_MAKE_ANIMATION()
	(
		VOLT_MAKE_MODULE(UVolt_ASM_InterpBackgroundColor)
		.TargetColor(OutlineNormalColor)
		.RateBasedInterpSpeed(45)
	);

	ColorTrack = VOLT_PLAY_ANIM(Button, Anim);
	OutlineColorTrack = VOLT_PLAY_ANIM(Border, BorderColorAnim);

	OnUnhovered.ExecuteIfBound();
}


void SVoltStarshipToggleButton::Construct(const FArguments& InArgs)
{
	OnColor = InArgs._OnColor;
	OffColor = InArgs._OffColor;

	OnPressed = InArgs._OnPressed;
	OnHovered = InArgs._OnHovered;
	OnUnhovered = InArgs._OnUnhovered;

	ButtonLength = InArgs._ButtonLength;


	ChildSlot[
		SAssignNew(Button, SButton)
		.ButtonStyle(InArgs._ButtonStyle)
		.ButtonColorAndOpacity(OffColor)
		.OnPressed(this, &SVoltStarshipToggleButton::EventOnPressed)
		.OnHovered(this, &SVoltStarshipToggleButton::EventOnHovered)
		.OnUnhovered(this, &SVoltStarshipToggleButton::EventOnUnhovered)
		.ContentPadding(FMargin(ButtonLength, 4))
		[
			SAssignNew(Toggle, SBorder)
			.BorderImage(InArgs._ToggleImage)
			.Padding(FMargin(InArgs._ToggleSize))
		]
	];

	PlayToggleAnimation(bIsToggled, true);
}

void SVoltStarshipToggleButton::PlayToggleAnimation(const bool& bToggled, const bool& bInstant = false)
{
	VOLT_STOP_ALL_ANIM(Button->AsShared());
	VOLT_STOP_ALL_ANIM(Toggle->AsShared());

	UVoltAnimation* Color = VOLT_MAKE_ANIMATION()
	(
		VOLT_MAKE_MODULE(UVolt_ASM_InterpBackgroundColor)
		.TargetColor(bToggled ? OnColor : OffColor)
		.RateBasedInterpSpeed(20)
	);

	UVoltAnimation* Transform = VOLT_MAKE_ANIMATION()
	(
		VOLT_MAKE_MODULE(UVolt_ASM_InterpWidgetTransform)
		.TargetWidgetTransform(FWidgetTransform(FVector2D(bToggled ? ButtonLength * 0.66 : ButtonLength * -0.66, 0),
		                                        FVector2D(1, 1), FVector2D(0, 0), 0))
		.RateBasedInterpSpeed(15)
	);

	VOLT_PLAY_ANIM(Button, Color);
	VOLT_PLAY_ANIM(Toggle, Transform);
}

void SVoltStarshipToggleButton::EventOnPressed()
{
	bIsToggled = !bIsToggled;

	PlayToggleAnimation(bIsToggled);

	OnPressed.ExecuteIfBound();
}

void SVoltStarshipToggleButton::EventOnHovered()
{
	OnHovered.ExecuteIfBound();
}

void SVoltStarshipToggleButton::EventOnUnhovered()
{
	OnUnhovered.ExecuteIfBound();
}


void SVoltStarshipScrollElement::Construct(const FArguments& InArgs)
{
	Content = InArgs._Content.Widget;

	AnimationManager = InArgs._AnimationManager;

	ChildSlot[
		SAssignNew(Border, SBorder)
		.BorderBackgroundColor(FLinearColor::Transparent)
		.Padding(2)
		.VAlign(VAlign_Fill)
		.HAlign(HAlign_Fill)
		[
			Content.ToSharedRef()
		]
	];

	PlayAnimation(InArgs._Interval);
}

void SVoltStarshipScrollElement::PlayAnimation(const double& Interval)
{
	VOLT_STOP_ALL_ANIM(AnimationManager, Border->AsShared());


	UVoltAnimation* Anim = VOLT_MAKE_ANIMATION()(
		VOLT_MAKE_MODULE(UVolt_ASM_Sequence)
		.bShouldLoop(false)
		(
			VOLT_MAKE_MODULE(UVolt_ASM_InterpRenderOpacity)
			.bUseStartOpacity(true)
			.StartOpacity(0)
			.TargetOpacity(0)
			.RateBasedInterpSpeed(-1),
			VOLT_MAKE_MODULE(UVolt_ASM_Delay)
			.Duration(Interval),
			VOLT_MAKE_MODULE(UVolt_ASM_Simultaneous)
			(
				VOLT_MAKE_MODULE(UVolt_ASM_InterpRenderOpacity)
				.bUseStartOpacity(true)
				.RateBasedInterpSpeed(5)
				.StartOpacity(0)
				.TargetOpacity(1),
				VOLT_MAKE_MODULE(UVolt_ASM_InterpWidgetTransform)
				.bUseStartWidgetTransform(true)
				.StartWidgetTransform(FWidgetTransform(FVector2D(100, 0), FVector2D(0.95, 0.95), FVector2D(0, 0), 0))
				.TargetWidgetTransform(FWidgetTransform(FVector2D(0, 0), FVector2D(0.95, 0.95), FVector2D(0, 0), 0))
				.RateBasedInterpSpeed(20)
			)
		)
	);


	VOLT_PLAY_ANIM(AnimationManager, Border->AsShared(), Anim);
}

void SVoltStarshipExpandingBorder::Construct(const FArguments& InArgs)
{
	DelayDuration = InArgs._DelayDuration;

	Content = InArgs._Content.Widget;

	ChildSlot[
		SAssignNew(Box, SBox)
		.VAlign(VAlign_Fill)
		.HAlign(HAlign_Fill)
		.WidthOverride(0)
		.HeightOverride(0)
		[
			Content.ToSharedRef()
		]
	];
	
}

void SVoltStarshipExpandingBorder::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime,
	const float InDeltaTime)
{
	SCompoundWidget::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);

	if(!bEverPlayedAnimation) PlayAnimation();
}

void SVoltStarshipExpandingBorder::PlayAnimation()
{
	VOLT_STOP_ALL_ANIM(Box->AsShared());
	VOLT_STOP_ALL_ANIM(Content->AsShared());

	const FVector2D& Size = Content.ToSharedRef()->GetDesiredSize();

	UVoltAnimation* BoxAnim = VOLT_MAKE_ANIMATION()(
		VOLT_MAKE_MODULE(UVolt_ASM_Sequence)
		(
			VOLT_MAKE_MODULE(UVolt_ASM_InterpBoxProperties)
			.bOverride_WidthOverride(true)
			.bUseStartWidthOverride(true)
			.bOverride_HeightOverride(true)
			.bUseStartHeightOverride(true)
			.StartWidthOverride(0)
			.TargetWidthOverride(0)
			.StartHeightOverride(0)
			.TargetHeightOverride(0)
			.RateBasedInterpSpeed(-1),
			VOLT_MAKE_MODULE(UVolt_ASM_Delay)
			.Duration(DelayDuration),
			VOLT_MAKE_MODULE(UVolt_ASM_InterpBoxProperties)
			.bOverride_WidthOverride(true)
			.bUseStartWidthOverride(true)
			.StartWidthOverride(0)
			.TargetWidthOverride(Size.X)
			.bOverride_HeightOverride(true)
			.bUseStartHeightOverride(true)
			.StartHeightOverride(0)
			.TargetHeightOverride(Size.Y)
			.RateBasedInterpSpeed(15)
		)
	);

	UVoltAnimation* ContentAnim = VOLT_MAKE_ANIMATION()(
		VOLT_MAKE_MODULE(UVolt_ASM_Sequence)
		(
			VOLT_MAKE_MODULE(UVolt_ASM_InterpRenderOpacity)
			.bUseStartOpacity(true)
			.StartOpacity(0)
			.TargetOpacity(0)
			.RateBasedInterpSpeed(-1),
			VOLT_MAKE_MODULE(UVolt_ASM_Delay)
			.Duration(DelayDuration),
			VOLT_MAKE_MODULE(UVolt_ASM_InterpRenderOpacity)
			.bUseStartOpacity(true)
			.StartOpacity(0)
			.TargetOpacity(1)
			.RateBasedInterpSpeed(10)
		)
	);

	VOLT_PLAY_ANIM(Box->AsShared(), BoxAnim);
	VOLT_PLAY_ANIM(Content->AsShared(), ContentAnim);

	bEverPlayedAnimation = true;
}

void SVoltStarshipDescriptionCard::Construct(const FArguments& InArgs)
{
	Content = InArgs._Content.Widget;
	
	ChildSlot[
		SAssignNew(Border, SBorder)
		.RenderTransformPivot(FVector2D(0.5, 0.5))
		.BorderImage(FVoltEditorStyle::Get().GetBrush("VoltStarship.Image.Box"))
		.BorderBackgroundColor(InArgs._ExplanationBackgroundColor)
		.VAlign(VAlign_Center)
		.HAlign(HAlign_Center)
		.Clipping(EWidgetClipping::ClipToBounds)
		[
			SNew(SOverlay)
			+ SOverlay::Slot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(SSpacer)
				.Size(InArgs._Size)
			]
			+ SOverlay::Slot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				Content.ToSharedRef()
			]
			+ SOverlay::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.FillWidth(0.3)
				.VAlign(VAlign_Fill)
				.HAlign(HAlign_Fill)
				[
					SAssignNew(DescriptionBorder, SBorder)
					.RenderTransformPivot(FVector2D(0.5, 0.5))
					.BorderBackgroundColor(InArgs._ExplanationSectionColor)
					.BorderImage(FVoltEditorStyle::Get().GetBrush("VoltStarship.Image.Box"))
					.Padding(2)
					.VAlign(VAlign_Fill)
					.HAlign(HAlign_Fill)
					[
						SNew(SVerticalBox)
						+ SVerticalBox::Slot()
						.AutoHeight()
						.Padding(2)
						[
							SNew(SBorder)
							.Clipping(EWidgetClipping::ClipToBounds)
							.BorderBackgroundColor(FLinearColor::Transparent)
							[
								SAssignNew(TitleTextBlock, STextBlock)
								.RenderTransformPivot(FVector2D(0.5, 1))
								.ColorAndOpacity(FLinearColor::White)
								.Text(InArgs._Title)
								.TextStyle(FVoltEditorStyle::Get(), "VoltStarship.Font.White")
								.AutoWrapText(true)
							]
						]
						+ SVerticalBox::Slot()
						.AutoHeight()
						.Padding(2)
						[
							SNew(SBorder)
							.Clipping(EWidgetClipping::ClipToBounds)
							.BorderBackgroundColor(FLinearColor::Transparent)
							[
								SAssignNew(DescriptionTextBlock, STextBlock)
								.RenderTransformPivot(FVector2D(0.5, 1))
								.ColorAndOpacity(FLinearColor::White)
								.Text(InArgs._Description)
								.AutoWrapText(true)
							]
						]
					]
				]
				+ SHorizontalBox::Slot()
				.FillWidth(0.7)
				.VAlign(VAlign_Fill)
				.HAlign(HAlign_Fill)
				[
					SNullWidget::NullWidget
				]
			]
		]
	];


	PlayInitAnimation();
}


void SVoltStarshipDescriptionCard::PlayInitAnimation()
{
	VOLT_STOP_ALL_ANIM(DescriptionBorder->AsShared());

	UVoltAnimation* Anim1 = VOLT_MAKE_ANIMATION()(
		VOLT_MAKE_MODULE(UVolt_ASM_InterpRenderOpacity)
		.InterpolationMode(EVoltInterpMode::AlphaBased)
		.AlphaBasedEasingFunction(EEasingFunc::CircularOut)
		.AlphaBasedDuration(1.2)
		.bUseStartOpacity(true)
		.StartOpacity(0)
		.TargetOpacity(1),
		VOLT_MAKE_MODULE(UVolt_ASM_InterpWidgetTransform)
		.InterpolationMode(EVoltInterpMode::AlphaBased)
		.AlphaBasedEasingFunction(EEasingFunc::CircularOut)
		.AlphaBasedDuration(1)
		.bUseStartWidgetTransform(true)
		.StartWidgetTransform(FWidgetTransform(FVector2D(-500, 0), FVector2D(1, 1.), FVector2D(0, 0), 0))
		.TargetWidgetTransform(FWidgetTransform(FVector2D(0, 0), FVector2D(1, 1.), FVector2D(0, 0), 0))
	);

	UVoltAnimation* Anim2 = VOLT_MAKE_ANIMATION()(
		VOLT_MAKE_MODULE(UVolt_ASM_Sequence)
		.bShouldLoop(false)
		(
			VOLT_MAKE_MODULE(UVolt_ASM_InterpWidgetTransform)
			.RateBasedInterpSpeed(0)
			.TargetWidgetTransform(FWidgetTransform(FVector2D(-300, 0), FVector2D(1.01, 1.01), FVector2D(0, 0), 0)),
			VOLT_MAKE_MODULE(UVolt_ASM_Delay)
			.Duration(1.2),
			VOLT_MAKE_MODULE(UVolt_ASM_InterpWidgetTransform)
			.InterpolationMode(EVoltInterpMode::AlphaBased)
			.AlphaBasedEasingFunction(EEasingFunc::EaseInOut)
			.AlphaBasedDuration(2)
			.bUseStartWidgetTransform(true)
			.StartWidgetTransform(FWidgetTransform(FVector2D(-300, 0), FVector2D(1.01, 1.01), FVector2D(0, 0), 0))
			.TargetWidgetTransform(FWidgetTransform(FVector2D(0, 0), FVector2D(1.01, 1.01), FVector2D(0, 0), 0))
		)
	);

	UVoltAnimation* Anim3 = VOLT_MAKE_ANIMATION()(
		VOLT_MAKE_MODULE(UVolt_ASM_Sequence)
		.bShouldLoop(false)
		(
			VOLT_MAKE_MODULE(UVolt_ASM_InterpWidgetTransform)
			.RateBasedInterpSpeed(0)
			.TargetWidgetTransform(FWidgetTransform(FVector2D(0, 150), FVector2D(1, 1), FVector2D(0, 0), 0)),
			VOLT_MAKE_MODULE(UVolt_ASM_Delay)
			.Duration(1.6),
			VOLT_MAKE_MODULE(UVolt_ASM_InterpWidgetTransform)
			.InterpolationMode(EVoltInterpMode::AlphaBased)
			.AlphaBasedEasingFunction(EEasingFunc::EaseInOut)
			.AlphaBasedDuration(2)
			.bUseStartWidgetTransform(true)
			.StartWidgetTransform(FWidgetTransform(FVector2D(0, 150), FVector2D(1, 1), FVector2D(0, 0), 0))
			.TargetWidgetTransform(FWidgetTransform(FVector2D(0, 0), FVector2D(1, 1), FVector2D(0, 0), 0))
		)
	);

	UVoltAnimation* Anim4 = VOLT_MAKE_ANIMATION()(
		VOLT_MAKE_MODULE(UVolt_ASM_Sequence)
		.bShouldLoop(false)
		(
			VOLT_MAKE_MODULE(UVolt_ASM_InterpWidgetTransform)
			.RateBasedInterpSpeed(0)
			.TargetWidgetTransform(FWidgetTransform(FVector2D(0, 150), FVector2D(1, 1), FVector2D(0, 0), 0)),
			VOLT_MAKE_MODULE(UVolt_ASM_Delay)
			.Duration(1.9),
			VOLT_MAKE_MODULE(UVolt_ASM_InterpWidgetTransform)
			.InterpolationMode(EVoltInterpMode::AlphaBased)
			.AlphaBasedEasingFunction(EEasingFunc::EaseInOut)
			.AlphaBasedDuration(2)
			.bUseStartWidgetTransform(true)
			.StartWidgetTransform(FWidgetTransform(FVector2D(0, 150), FVector2D(1, 1), FVector2D(0, 0), 0))
			.TargetWidgetTransform(FWidgetTransform(FVector2D(0, 0), FVector2D(1, 1), FVector2D(0, 0), 0))
		)
	);

	VOLT_PLAY_ANIM(Border->AsShared(), Anim1);

	VOLT_PLAY_ANIM(DescriptionBorder->AsShared(), Anim2);

	VOLT_PLAY_ANIM(TitleTextBlock->AsShared(), Anim3);

	VOLT_PLAY_ANIM(DescriptionTextBlock->AsShared(), Anim4);
}


static TAutoConsoleVariable<float> StaticToolTipWrapWidthVolt(
	TEXT("Slate.ToolTipWrapWidth"),
	1000.0f,
	TEXT("Width of Slate tool-tips before we wrap the tool-tip text"));


float SVoltStarshipTooltip::GetToolTipWrapWidth()
{
	return StaticToolTipWrapWidthVolt.GetValueOnAnyThread();
}


void SVoltStarshipTooltip::Construct(const FArguments& InArgs)
{
	TextContent = InArgs._Text;
	bIsInteractive = InArgs._IsInteractive;
	Font = InArgs._Font;
	TextMargin = InArgs._TextMargin;
	BorderImage = InArgs._BorderImage;
	OnSetInteractiveWindowLocationDelegate = InArgs._OnSetInteractiveWindowLocation;
	OnVoltStarshipTooltipOpened = InArgs._OnVoltStarshipTooltipOpened;

	SetContentWidget(InArgs._Content.Widget);
}


void SVoltStarshipTooltip::SetContentWidget(const TSharedRef<SWidget>& InContentWidget)
{
	if (InContentWidget != SNullWidget::NullWidget)
	{
		// Widget content argument takes precedence over the text content.
		WidgetContent = InContentWidget;
	}

	TSharedPtr<SWidget> PinnedWidgetContent = WidgetContent.Pin();
	if (PinnedWidgetContent.IsValid())
	{
		ToolTipContent = PinnedWidgetContent;

		// Tool-tip with entirely custom content.  We'll create a border with some padding (as customized by the user), then
		// embed their custom widget right inside the border.  This tool-tip currently has a different styling than tool-tips
		// that only contain text.
		ChildSlot
		[
			SNew(SBorder)
			.BorderBackgroundColor(FLinearColor::Transparent)
			[
				ToolTipContent.ToSharedRef()
			]
		];
	}
	else
	{
		ToolTipContent =
			SNew(STextBlock)
			.Text(TextContent)
			.Font(Font)
			.ColorAndOpacity(FLinearColor::Black)
			.WrapTextAt_Static(&SToolTip::GetToolTipWrapWidth);

		// Text-only tool-tip.  This tool-tip currently has a different styling than tool-tips with custom content.  We always want basic
		// text tool-tips to look consistent.
		ChildSlot
		[
			SNew(SBorder)
			.BorderBackgroundColor(FLinearColor::Transparent)
			[
				ToolTipContent.ToSharedRef()
			]
		];
	}
}

void SVoltStarshipTooltip::ResetContentWidget()
{
	WidgetContent.Reset();
	SetContentWidget(SNullWidget::NullWidget);
}

bool SVoltStarshipTooltip::IsEmpty() const
{
	return !WidgetContent.IsValid() && TextContent.Get().IsEmpty();
}


bool SVoltStarshipTooltip::IsInteractive() const
{
	return bIsInteractive.Get();
}

void SVoltStarshipTooltip::OnOpening()
{
	OnVoltStarshipTooltipOpened.ExecuteIfBound(StaticCastSharedRef<SVoltStarshipTooltip, SWidget>(this->AsShared()));
}

void SVoltStarshipTooltip::OnClosed()
{
}

void SVoltStarshipTooltip::OnSetInteractiveWindowLocation(FVector2D& InOutDesiredLocation) const
{
	if (OnSetInteractiveWindowLocationDelegate.IsBound())
	{
		OnSetInteractiveWindowLocationDelegate.Execute(InOutDesiredLocation);
	}
}

#undef LOCTEXT_NAMESPACE
#undef VOLT_STARSHIP_BORDER_MARGIN

#undef VOLT_STARSHIP_BORDER_BACKGROUND_COLOR
#undef VOLT_STARSHIP_BORDER_BACKGROUND_COLOR_BRIGHT
#undef VOLT_STARSHIP_BORDER_BACKGROUND_COLOR_BRIGHT_HOVER
