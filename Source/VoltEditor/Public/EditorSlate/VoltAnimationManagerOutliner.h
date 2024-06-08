//Copyright 2022~2024 DevGrain. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "VoltAnimationTrack.h"
#include "Widgets/SCompoundWidget.h"

class SScrollBox;
class SWrapBox;
class SCheckBox;
class STextBlock;

class UVoltModuleItem;
class UVoltAnimationManager;

class VOLTEDITOR_API SVoltAnimationManagerOutliner_Module : public SCompoundWidget
{

public:

	SLATE_BEGIN_ARGS(SVoltAnimationManagerOutliner_Module) {}
		SLATE_ARGUMENT(TObjectPtr<UVoltModuleItem>, TargetModule)
		SLATE_ARGUMENT(TObjectPtr<UVoltAnimationManager>, InternalSlateAnimationManager) //for the editor slate animation
	SLATE_END_ARGS();
	
public:

	void Construct(const FArguments& InArgs);
	void RebuildWidget();
	void UpdateWidget();

public:

	//The module that this element refer.
	
	TObjectPtr<UVoltModuleItem> Module;
	
	TSharedPtr<SCheckBox> ActivityBox;

	TObjectPtr<UVoltAnimationManager> InternalSlateAnimationManager;
	
};


class VOLTEDITOR_API SVoltAnimationManagerOutliner_AnimTrack : public SCompoundWidget
{

public:

	SLATE_BEGIN_ARGS(SVoltAnimationManagerOutliner_AnimTrack)
	{
		_InternalSlateAnimationManager = nullptr;
	}
		SLATE_ARGUMENT(FVoltAnimationTrack, TargetTrack)
		SLATE_ARGUMENT(TObjectPtr<UVoltAnimationManager>, InternalSlateAnimationManager) //for the editor slate animation
	SLATE_END_ARGS();
public:

	void Construct(const FArguments& InArgs);
	void RebuildWidget();

	void PopulateModuleElementWidget();

	void UpdateWidget();

public:

	//A copy of the track that this element refer.
	FVoltAnimationTrack Track;

	TArray<TSharedPtr<SVoltAnimationManagerOutliner_Module>> ModuleElems;

	TSharedPtr<SScrollBox> ModuleBox;

	TObjectPtr<UVoltAnimationManager> InternalSlateAnimationManager;
};


class VOLTEDITOR_API SVoltAnimationManagerOutliner : public SCompoundWidget
{
	
public:

	SLATE_BEGIN_ARGS(SVoltAnimationManagerOutliner) {}
		SLATE_ARGUMENT(TObjectPtr<UVoltAnimationManager>, AnimationManager)
	SLATE_END_ARGS();
	
public:

	~SVoltAnimationManagerOutliner();
	
public:

	void Construct(const FArguments& InArgs);
	void RebuildWidget();

public:

	//Layout
	TSharedRef<SWidget> CreateVoltListSection();

public:
	

	TSharedRef<SWidget> CreateAnimationTrackListSection();

	void UpdateAnimationTrack();
	
	//TArray<TTuple<FVoltAnimationTrack,TWeakPtr<SWidget>>> TrackElementWidgets;
	TMap<FVoltAnimationTrack,TSharedPtr<SVoltAnimationManagerOutliner_AnimTrack>> TrackElementWidgets;

	TSharedPtr<SScrollBox> TrackElementWidgetBox;


public:
	
	UVoltAnimationManager* GetAnimationManager();

	void SetAnimationManager(TObjectPtr<UVoltAnimationManager> Manager);


public:
	
	TObjectPtr<UVoltAnimationManager> ManagerPtr;

	//For the animations of the inherited slates.
	TObjectPtr<UVoltAnimationManager> InternalSlateAnimationManagerPtr;

};

