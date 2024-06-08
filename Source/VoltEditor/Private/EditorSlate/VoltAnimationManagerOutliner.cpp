//Copyright 2022~2024 DevGrain. All Rights Reserved.

#include "VoltAnimationManagerOutliner.h"

#include "DiffUtils.h"
#include "VoltEditorStyle.h"
#include "Components/VerticalBox.h"
#include "VoltAnimation.h"

#include "VoltAnimationManager.h"
#include "VoltInterface.h"
#include "VoltModuleItem.h"
#include "Variables/VoltVariables.h"
#include "Animation/Volt_ASA_Emerge.h"
#include "Animation/Volt_ASA_Expand.h"
#include "Animation/Volt_ASA_Shrink.h"
#include "Slate/SVoltAnimatedBorder.h"

#include "Widgets/Input/SCheckBox.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Layout/SWrapBox.h"

void SVoltAnimationManagerOutliner_Module::Construct(const FArguments& InArgs)
{
	Module = InArgs._TargetModule;

	InternalSlateAnimationManager = InArgs._InternalSlateAnimationManager;

	RebuildWidget();
}

void SVoltAnimationManagerOutliner_Module::RebuildWidget()
{
	const TAttribute<ECheckBoxState> CheckedAttr = TAttribute<ECheckBoxState>::Create(
		TAttribute<ECheckBoxState>::FGetter::CreateLambda([this]
		{
			if (Module != nullptr)
			{
				return (Module->IsActive()) ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
			}
			return ECheckBoxState::Undetermined;
		}));

	this->ChildSlot.DetachWidget();

	this->ChildSlot[
		SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.AutoWidth()
		[
			SNew(STextBlock)
			.Text(FText::FromString(Module->GetName()))
		]
		+ SHorizontalBox::Slot()
		.AutoWidth()
		[
			SNew(SCheckBox)
			.IsChecked(CheckedAttr)
		]
	];
}

void SVoltAnimationManagerOutliner_Module::UpdateWidget()
{
}


void SVoltAnimationManagerOutliner_AnimTrack::Construct(const FArguments& InArgs)
{
	Track = InArgs._TargetTrack;

	InternalSlateAnimationManager = InArgs._InternalSlateAnimationManager;

	RebuildWidget();
}

void SVoltAnimationManagerOutliner_AnimTrack::RebuildWidget()
{
	if (!Track.TargetAnimation) return;
	if (!Track.TargetSlateInterface.GetInterface()) return;
	if (Track.TargetSlateInterface.GetInterface()->GetTargetSlate() == nullptr) return;
	if (!Track.TargetAnimation.IsValid() || !Track.TargetSlateInterface.GetInterface()->GetTargetSlate().IsValid())
		return;

	this->ChildSlot.DetachWidget();


	TSharedPtr<SVoltAnimatedBorder> Border = SNew(SVoltAnimatedBorder,
			SBorder::FArguments()
			.Padding(FMargin(4))
			.BorderImage(FVoltEditorStyle::Get().GetBrush("VoltEditorStyle.RoundedBox.Test"))
		)
		.ParentAnimationManager(InternalSlateAnimationManager)
		.OnMouseEnterAnimationClass(UVolt_ASA_Expand::StaticClass())
		.OnMouseLeaveAnimationClass(UVolt_ASA_Shrink::StaticClass());

	Border->SetContent(
		SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.AutoWidth()
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(STextBlock) //Animation name
				.Text(FText::FromString(Track.TargetAnimation->GetName()))
			]
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				Track.TargetSlateInterface.GetInterface()->GetTargetSlate().Pin().ToSharedRef()
			]
		]
		+ SHorizontalBox::Slot()
		  .VAlign(VAlign_Center)
		  .HAlign(HAlign_Right)
		  .FillWidth(1)
		[
			SAssignNew(ModuleBox, SScrollBox) //Animation name
		]);

	this->ChildSlot[
		Border.ToSharedRef()
	];

	VOLT_PLAY_ANIM(
		InternalSlateAnimationManager,
		VOLT_FIND_OR_ASSIGN_SLATE(InternalSlateAnimationManager, Border),
		VOLT_GET_ANIMATION<UVolt_ASA_Emerge>(UVolt_ASA_Emerge::StaticClass()));


	PopulateModuleElementWidget();
}

void SVoltAnimationManagerOutliner_AnimTrack::PopulateModuleElementWidget()
{
	if (!ModuleBox.IsValid()) return;
	if (!Track.TargetAnimation) return;
	if (!Track.TargetAnimation.IsValid()) return;

	for (UVoltModuleItem* Module : Track.TargetAnimation->Modules)
	{
		if (Module == nullptr) continue;

		ModuleBox->AddSlot()
		[
			SNew(SVoltAnimationManagerOutliner_Module)
			.TargetModule(Module)
		];
	}
}

void SVoltAnimationManagerOutliner_AnimTrack::UpdateWidget()
{
	if (!ModuleBox.IsValid()) return;

	for (const TSharedPtr<SVoltAnimationManagerOutliner_Module>& AnimationManagerOutliner_Module : ModuleElems)
	{
		AnimationManagerOutliner_Module->UpdateWidget();
	}
}

SVoltAnimationManagerOutliner::~SVoltAnimationManagerOutliner()
{
	VOLT_RELEASE_MANAGER(&InternalSlateAnimationManagerPtr);
}

void SVoltAnimationManagerOutliner::Construct(const FArguments& InArgs)
{
	ManagerPtr = InArgs._AnimationManager;

	VOLT_IMPLEMENT_MANAGER(&InternalSlateAnimationManagerPtr);

	RebuildWidget();
}

TSharedRef<SWidget> SVoltAnimationManagerOutliner::CreateAnimationTrackListSection()
{
	TrackElementWidgetBox = SNew(SScrollBox).Orientation(EOrientation::Orient_Vertical);

	UpdateAnimationTrack();

	return TrackElementWidgetBox.ToSharedRef();
}

void SVoltAnimationManagerOutliner::UpdateAnimationTrack()
{
	if (GetAnimationManager() == nullptr) return;
	if (!GetAnimationManager()->IsValidLowLevel()) return;


	//Revert if the box is invalid.
	if (!TrackElementWidgetBox.IsValid()) return;

	const TArray<FVoltAnimationTrack>& Tracks = GetAnimationManager()->GetAnimationTracks();

	TArray<FVoltAnimationTrack> Keys;
	TrackElementWidgets.GetKeys(Keys);

	//Remove invalid track elements
	for (const FVoltAnimationTrack& Key : Keys)
	{
		if (Tracks.Contains(Key)) continue;

		//Make sure we already implemented the widget.
		if (!TrackElementWidgets.Contains(Key)) continue;
		TrackElementWidgetBox->RemoveSlot(TrackElementWidgets[Key].ToSharedRef());
		TrackElementWidgets.Remove(Key);
	}

	//Implement new track elements
	for (const FVoltAnimationTrack& VoltAnimationTrack : Tracks)
	{
		if (TrackElementWidgets.Contains(VoltAnimationTrack)) continue;

		TSharedRef<SVoltAnimationManagerOutliner_AnimTrack> NewElemWidget = SNew(SVoltAnimationManagerOutliner_AnimTrack)
			.InternalSlateAnimationManager(InternalSlateAnimationManagerPtr)
			.TargetTrack(VoltAnimationTrack);

		TrackElementWidgetBox->AddSlot()
		.Padding(FMargin(4))
		[
			NewElemWidget
		];

		TrackElementWidgets.Add(VoltAnimationTrack, NewElemWidget);
	}

	//Update the element widgets
	for (TTuple<FVoltAnimationTrack, TSharedPtr<SVoltAnimationManagerOutliner_AnimTrack>> TrackElementWidget :
	     TrackElementWidgets)
	{
		TrackElementWidget.Value->UpdateWidget();
	}
}

TSharedRef<SWidget> SVoltAnimationManagerOutliner::CreateVoltListSection()
{
	TSharedRef<SVerticalBox> Box = SNew(SVerticalBox);

	const TArray<TScriptInterface<IVoltInterface>>& Slates = GetAnimationManager()->GetVolts();

	for (const TScriptInterface<IVoltInterface>& VoltInterface : Slates)
	{
		FString InterfaceInstanceName = "Not derived from UObject type; It might be originated from native class.";
		FString SlateName = "";

		if (!VoltInterface->GetTargetSlate().IsValid()) continue;

		SlateName = FString::FromInt(VoltInterface->GetTargetSlate().Pin()->GetId());

		if (VoltInterface.GetObject())
		{
			InterfaceInstanceName = VoltInterface.GetObject()->GetName();
		}

		Box->AddSlot()
		   .AutoHeight()
		[
			SNew(STextBlock)
			.Text(FText::FromString(SlateName))
		];
	}

	return Box;
}

void SVoltAnimationManagerOutliner::RebuildWidget()
{
	this->ChildSlot.DetachWidget();

	this->ChildSlot
	[
		CreateAnimationTrackListSection()
	];
}

UVoltAnimationManager* SVoltAnimationManagerOutliner::GetAnimationManager()
{
	return ManagerPtr;
}


void SVoltAnimationManagerOutliner::SetAnimationManager(TObjectPtr<UVoltAnimationManager> Manager)
{
	ManagerPtr = Manager;
}
