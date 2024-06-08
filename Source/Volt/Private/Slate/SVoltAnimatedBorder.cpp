//Copyright 2022~2024 DevGrain. All Rights Reserved.

#include "SVoltAnimatedBorder.h"

#include "VoltAnimation.h"
#include "VoltAnimationManager.h"


void SVoltAnimatedBorder::Construct(const SVoltAnimatedBorder::FArguments& InArgs, SBorder::FArguments BorderArgs)
{
	BorderArgs._Content = InArgs._Content;
	ParentAnimationManager = InArgs._ParentAnimationManager;

	CreateAnimationManagerIfNeeded();
	
	OnMouseEnterAnimationClass = InArgs._OnMouseEnterAnimationClass;
	OnMouseLeaveAnimationClass = InArgs._OnMouseLeaveAnimationClass;

	SBorder::Construct(BorderArgs);
}

SVoltAnimatedBorder::~SVoltAnimatedBorder()
{
	VOLT_RELEASE_MANAGER(&InternalAnimationManager);
}

void SVoltAnimatedBorder::CreateAnimationManagerIfNeeded()
{
	if(ParentAnimationManager != nullptr) return;

	VOLT_IMPLEMENT_MANAGER(&InternalAnimationManager);
}

void SVoltAnimatedBorder::OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	SBorder::OnMouseEnter(MyGeometry, MouseEvent);

	VOLT_STOP_ANIM(
		GetAnimationManager(),
		MouseLeaveAnimTrack);

	MouseEnterAnimTrack = VOLT_PLAY_ANIM(
		GetAnimationManager(),
		VOLT_FIND_OR_ASSIGN_SLATE(GetAnimationManager(), this->AsShared()),
		VOLT_GET_ANIMATION(OnMouseEnterAnimationClass,GetAnimationManager()));
}


FReply SVoltAnimatedBorder::OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	return SBorder::OnMouseMove(MyGeometry, MouseEvent);
}

void SVoltAnimatedBorder::OnMouseLeave(const FPointerEvent& MouseEvent)
{
	SBorder::OnMouseLeave(MouseEvent);

	VOLT_STOP_ANIM(
		GetAnimationManager(),
		MouseEnterAnimTrack);
	
	MouseLeaveAnimTrack = VOLT_PLAY_ANIM(
		GetAnimationManager(),
		VOLT_FIND_OR_ASSIGN_SLATE(GetAnimationManager(), this->AsShared()),
		VOLT_GET_ANIMATION(OnMouseLeaveAnimationClass,GetAnimationManager()));
}

UVoltAnimationManager* SVoltAnimatedBorder::GetAnimationManager() const
{
	return (ParentAnimationManager != nullptr) ? ParentAnimationManager
		: (InternalAnimationManager != nullptr) ? InternalAnimationManager
		: nullptr;
}
