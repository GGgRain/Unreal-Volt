//Copyright 2022~2024 DevGrain. All Rights Reserved.

#include "VoltAnimationManager.h"
#include "VoltAnimation.h"
#include "VoltAnimationTrack.h"
#include "VoltInterface.h"
#include "VoltModuleItem.h"
#include "VoltProxy.h"
#include "VoltSettings.h"
#include "VoltVariableBase.h"
#include "VoltVariableCollection.h"

TArray<FVoltAnimationTrack> UVoltAnimationManager::PlayAnimationForAll(const UVoltAnimation* Animation)
{
	TArray<FVoltAnimationTrack> NewTracks;

	for (const TScriptInterface<IVoltInterface>& AnimatedCharacterSlate : Volts)
	{
		const FVoltAnimationTrack& NewTrack = PlayAnimationFor(AnimatedCharacterSlate, Animation);

		NewTracks.Add(NewTrack);
	}

	return NewTracks;
}


FVoltAnimationTrack UVoltAnimationManager::PlayAnimationFor(TScriptInterface<IVoltInterface> SlateInterface,
                                                            const UVoltAnimation* Animation)
{

#if WITH_EDITOR
	if (!Animation)
	{
		UE_LOG(LogTemp, Error, TEXT("An empty animation has been requested to be played. Reverted the action."));
	}
#endif

	if (!Animation) return FVoltAnimationTrack::NullTrack; // Revert of there is no animation to play.

#if WITH_EDITOR
	if (SlateInterface == nullptr)
	{
		UE_LOG(LogTemp, Error,
		       TEXT("An invalid slate interface has been requested to play an animation for. Reverted the action."));
	}
#endif

	if (SlateInterface == nullptr) return FVoltAnimationTrack::NullTrack;
	// Revert if there is no widget to apply the animation

	if (!CheckHasSlate(SlateInterface)) AssignSlate(SlateInterface);

	UVoltAnimation* AnimationInstance = DuplicateObject<UVoltAnimation>(Animation, this);

	for (UVoltModuleItem* Module : AnimationInstance->Modules)
	{
		if (!Module) continue;

		Module->SetVoltSlate(SlateInterface);
	}


	//Duplicate the modules.
	/*
	AnimationInstance->Modules.Empty();

	AnimationInstance->Modules.Reserve(Animation->Modules.Num());
	
	for (const UVoltModuleItem* Module : Animation->Modules)
	{
		if(Module == nullptr) continue;
		UVoltModuleItem* DuplicatedModule = DuplicateObject<UVoltModuleItem>(Module, this);

		//Set the module to work with the slate.
		DuplicatedModule->SetVolt(SlateInterface);
		
		AnimationInstance->Modules.Emplace(DuplicatedModule);
	}
	*/

	if (!AnimationInstance) return FVoltAnimationTrack::NullTrack;

	const FVoltAnimationTrack& Track = AnimationTracks.Add_GetRef(
		FVoltAnimationTrack(SlateInterface, AnimationInstance));

	//Broadcast the event.
	if (OnTrackAdded.IsBound()) OnTrackAdded.Broadcast(this, Track);

	//Broadcast the event.
	if (OnAnimationPlayed.IsBound()) OnAnimationPlayed.Broadcast(this, Track, Track.TargetAnimation.Get());

	return Track;
}

FVoltAnimationTrack UVoltAnimationManager::PlayAnimationAt(const int& Index, const UVoltAnimation* Animation)
{
	if (!Volts.IsValidIndex(Index)) return FVoltAnimationTrack::NullTrack;
	return PlayAnimationFor(Volts[Index], Animation);
}

void UVoltAnimationManager::TickAnimations(float DeltaTime)
{
	for (FVoltAnimationTrack& AnimationTrack : AnimationTracks)
	{
		if (!AnimationTrack.TargetAnimation) continue;

		if (!AnimationTrack.TargetSlateInterface) continue;

		for (UVoltModuleItem* Module : AnimationTrack.TargetAnimation->Modules)
		{
			if (!Module)
			{
#if WITH_EDITOR
				UE_LOG(LogTemp, Log, TEXT("There is a empty module slot in the %s. Please check out the asset.")
				       , *AnimationTrack.TargetAnimation.Get()->GetPathName());
#endif

				continue;
			}

			if (!AnimationTrack.TargetSlateInterface.GetObject())
			{
#if WITH_EDITOR
				UE_LOG(LogTemp, Log,
				       TEXT(
					       "VoltAnimationManager %s detected a slate interface that was not derived from UObject has been provided. Make sure to derive it from UObject."
				       ), *this->GetName());
#endif

				continue;
			}

			if (!Module->IsActive()) continue;

			Module->ModifySlateVariable(DeltaTime, AnimationTrack.TargetSlateInterface);
		}

		UVoltVariableCollection* Collection = AnimationTrack.TargetSlateInterface->
			GetVoltVariableCollection();

		if (!Collection) continue;

		for (UVoltVariableBase* Variable : Collection->GetVariables())
		{
			Variable->ApplyVariable(AnimationTrack.TargetSlateInterface->GetTargetSlate());
		}
	}
}

void UVoltAnimationManager::FlushUnnecessaryAnimation()
{
	const int num = AnimationTracks.Num();

	//Iterate the array backward.
	for (int index = num - 1; index >= 0; --index)
	{
		const FVoltAnimationTrack& TrackRef = AnimationTracks[index];

		if (!TrackRef.TargetAnimation)
		{
			FlushTrackAt(index);
			continue;
		}

		if (!TrackRef.TargetAnimation->IsActive())
		{
			FlushTrackAt(index);
			continue;
		}
	}
}

void UVoltAnimationManager::UpdateAnimations(float DeltaTime)
{
	TickAnimations(DeltaTime);

	FlushUnnecessaryAnimation();
}

bool UVoltAnimationManager::HasTrack(const FVoltAnimationTrack& Track)
{
	const int FoundIndex = AnimationTracks.Find(Track);

	//Revert if it was unable to find the index of the target.
	if (FoundIndex == INDEX_NONE) return false;

	return true;
}

void UVoltAnimationManager::FlushTrack(const FVoltAnimationTrack& Track)
{
	const int FoundIndex = AnimationTracks.Find(Track);

	//Revert if it was unable to find the index of the target.
	if (FoundIndex == INDEX_NONE) return;

	FlushTrackAt(FoundIndex);
}

void UVoltAnimationManager::FlushTrackAt(const int Index)
{
	if (!AnimationTracks.IsValidIndex(Index)) return;

	//Get the reference of the track from the array and use it instead of the provided track to avoid corruption.
	FVoltAnimationTrack& TrackRefToRemove = AnimationTracks[Index];

	//Broadcast the event.
	if (OnAnimationEnded.IsBound())
		OnAnimationEnded.Broadcast(this, TrackRefToRemove,
		                           TrackRefToRemove.TargetAnimation.Get());
	//Broadcast the event.
	if (OnTrackRemoved.IsBound()) OnTrackRemoved.Broadcast(this, TrackRefToRemove);


	//Release its animation.
	if (TrackRefToRemove.TargetAnimation)
	{
		//Release its animation's modules.
		/*
		for (UVoltModuleItem* Module : TrackRefToRemove.TargetAnimation->Modules)
		{
			if (!Module) continue;
			Module->Rename(nullptr, GetTransientPackage());
			Module->MarkAsGarbage();
		}
		TrackRefToRemove.TargetAnimation->Modules.Empty();
		
		//for some unfortunate occasions.
		if(TrackRefToRemove.TargetAnimation->IsRooted()) TrackRefToRemove.TargetAnimation->RemoveFromRoot();
		TrackRefToRemove.TargetAnimation->Rename(nullptr, GetTransientPackage());
		if(!TrackRefToRemove.TargetAnimation->IsRooted()) TrackRefToRemove.TargetAnimation->MarkAsGarbage();
		TrackRefToRemove.TargetAnimation = nullptr;
		*/
		TrackRefToRemove.TargetAnimation->Modules.Empty();
		TrackRefToRemove.TargetAnimation = nullptr;
	}

	AnimationTracks.RemoveAt(Index);
}

void UVoltAnimationManager::FlushTracksFor(TScriptInterface<IVoltInterface> TargetSlateInterface)
{
	if (!CheckHasSlate(TargetSlateInterface)) return;

	const int num = AnimationTracks.Num();

	//Iterate the array backward.
	for (int index = num - 1; index >= 0; --index)
	{
		if (AnimationTracks[index].TargetSlateInterface == TargetSlateInterface) FlushTrack(AnimationTracks[index]);
	}
}


void UVoltAnimationManager::FlushAllTracks()
{
	const int num = AnimationTracks.Num();

	for (int index = num - 1; index >= 0; --index)
	{
		FlushTrackAt(index);
	}

	AnimationTracks.Empty();
}

const TArray<FVoltAnimationTrack>& UVoltAnimationManager::GetAnimationTracks()
{
	return AnimationTracks;
}

const TArray<TScriptInterface<IVoltInterface>>& UVoltAnimationManager::GetVolts()
{
	return Volts;
}

TScriptInterface<IVoltInterface> UVoltAnimationManager::AssignSlate(
	TScriptInterface<IVoltInterface> SlateToAdd)
{
	const TScriptInterface<IVoltInterface> FoundSlate = FindSlateFor(SlateToAdd);
	if (FoundSlate != nullptr) return FoundSlate;

	Volts.Add(SlateToAdd);

	return SlateToAdd;
}

TScriptInterface<IVoltInterface> UVoltAnimationManager::AssignSlate(
	const TWeakPtr<SWidget>& SlateToAdd)
{
	const TScriptInterface<IVoltInterface> FoundSlate = FindSlateFor(SlateToAdd);
	if (FoundSlate != nullptr) return FoundSlate;

	UVoltProxy* Proxy = NewObject<UVoltProxy>(this);

	Proxy->Widget = SlateToAdd;

	Volts.Add(Proxy);

	return Proxy;
}

TScriptInterface<IVoltInterface> UVoltAnimationManager::FindSlateFor(
	const TScriptInterface<IVoltInterface>& SlateInterfaceToCheck) const
{
	return (Volts.Contains(SlateInterfaceToCheck)) ? SlateInterfaceToCheck : nullptr;
}

TScriptInterface<IVoltInterface> UVoltAnimationManager::FindSlateFor(
	const TWeakPtr<SWidget>& SlateToFind) const
{
	for (const TScriptInterface<IVoltInterface>& VoltInterface : Volts)
	{
		if (VoltInterface->GetTargetSlate() == SlateToFind) return VoltInterface;
	}
	return nullptr;
}

TScriptInterface<IVoltInterface> UVoltAnimationManager::FindOrAssignSlate(
	const TScriptInterface<IVoltInterface>& SlateInterfaceToAdd)
{
	const TScriptInterface<IVoltInterface> FoundSlate = FindSlateFor(SlateInterfaceToAdd);
	if (FoundSlate != nullptr) return FoundSlate;

	return AssignSlate(SlateInterfaceToAdd);
}


TScriptInterface<IVoltInterface> UVoltAnimationManager::FindOrAssignSlate(
	const TWeakPtr<SWidget>& SlateToFind)
{
	const TScriptInterface<IVoltInterface> FoundSlate = FindSlateFor(SlateToFind);
	if (FoundSlate != nullptr) return FoundSlate;

	return AssignSlate(SlateToFind);
}

bool UVoltAnimationManager::CheckHasSlate(
	const TScriptInterface<IVoltInterface>& SlateInterfaceToCheck) const
{
	return (FindSlateFor(SlateInterfaceToCheck) != nullptr);
}


bool UVoltAnimationManager::CheckHasSlate(const TWeakPtr<SWidget>& SlateToCheck) const
{
	return (FindSlateFor(SlateToCheck) != nullptr);
}

int UVoltAnimationManager::GetSlateCount() const
{
	return Volts.Num();
}

bool UVoltAnimationManager::IsPlayingAnimation()
{
	return !AnimationTracks.IsEmpty();
}

bool UVoltAnimationManager::CheckSlatePlayingAnimation(
	const TScriptInterface<IVoltInterface>& SlateInterfaceToCheck)
{
	if (!CheckHasSlate(SlateInterfaceToCheck)) return false;

	for (const FVoltAnimationTrack& AnimationTrack : AnimationTracks)
	{
		if (AnimationTrack.TargetSlateInterface == SlateInterfaceToCheck) return true;
	}

	return false;
}

bool UVoltAnimationManager::CheckSlatePlayingAnimation(const TWeakPtr<SWidget>& SlateToCheck)
{
	return CheckSlatePlayingAnimation(FindSlateFor(SlateToCheck));
}

void UVoltAnimationManager::Tick(float DeltaTime)
{
	UpdateAnimations(DeltaTime);
}

void UVoltAnimationManager::ReleaseAll()
{
	//GC will handle the leftover for it.

	FlushAllTracks();
	Volts.Empty();
}

bool UVoltAnimationManager::CheckShouldDestruct() const
{
	UVoltSubsystem* Subsystem = UVoltSubsystem::Get();

	if (GetOuter() && GetOuter() != Subsystem) return false;

	for (TScriptInterface<IVoltInterface> VoltInterface : Volts)
	{
		if (VoltInterface->GetTargetSlate().IsValid()) return false;
	}

	return true;
}

void UVoltAnimationManager::DestructSelf()
{
	ReleaseAll();

	MarkAsGarbage();
}

UWorld* UVoltAnimationManager::GetWorld() const
{
	if(GetOuter())
	{
		return GetOuter()->GetWorld();
	}
	
	return nullptr;
}

bool UVoltAnimationManager::CheckUseManualTick()
{
	return bUseManualTick;
}

void UVoltAnimationManager::SetUseManualTick(bool NewUseManualTick)
{
	bUseManualTick = NewUseManualTick;
}
