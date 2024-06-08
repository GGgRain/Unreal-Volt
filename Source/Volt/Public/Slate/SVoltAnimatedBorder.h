//Copyright 2022~2024 DevGrain. All Rights Reserved.

#pragma once

#include "VoltAnimationTrack.h"
#include "Widgets/Layout/SBorder.h"

class UVoltAnimation;
class UVoltAnimationManager;

class VOLT_API SVoltAnimatedBorder : public SBorder
{

	SLATE_BEGIN_ARGS(SVoltAnimatedBorder)
	{
		_ParentAnimationManager = nullptr; //Make sure this is nullptr at the beginning.
		_OnMouseEnterAnimationClass = nullptr;
		_OnMouseLeaveAnimationClass = nullptr;
	}

	//Toss it to the SBorder::Construct.
	SLATE_DEFAULT_SLOT( FArguments, Content )

	SLATE_ARGUMENT(TObjectPtr<UVoltAnimationManager>, ParentAnimationManager) // optional, if we don't provide any, it will create its own.
	SLATE_ARGUMENT(UClass*, OnMouseEnterAnimationClass) 
	SLATE_ARGUMENT(UClass*, OnMouseLeaveAnimationClass) 

	SLATE_END_ARGS()

public:

	~SVoltAnimatedBorder();
	
public:
	
	void CreateAnimationManagerIfNeeded();
	void Construct(const SVoltAnimatedBorder::FArguments& InArgs, SBorder::FArguments BorderArgs);

public:

	virtual void OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual void OnMouseLeave(const FPointerEvent& MouseEvent) override;

public:
	
	UVoltAnimationManager* GetAnimationManager() const;

public:
	
	TObjectPtr<UVoltAnimationManager> ParentAnimationManager = nullptr;
	TObjectPtr<UVoltAnimationManager> InternalAnimationManager = nullptr;

	UClass* OnMouseEnterAnimationClass = nullptr;
	UClass* OnMouseLeaveAnimationClass = nullptr;

	FVoltAnimationTrack MouseEnterAnimTrack;
	FVoltAnimationTrack MouseLeaveAnimTrack;

};
