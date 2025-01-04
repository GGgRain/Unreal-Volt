//Copyright 2022~2024 DevGrain. All Rights Reserved.

#include "VoltAnimationManager.h"
#include "VoltAnimation.h"
#include "VoltAnimationTrack.h"
#include "VoltInterface.h"
#include "VoltModuleItem.h"
#include "VoltVariableBase.h"
#include "VoltVariableCollection.h"


FVoltAnimationTrack UVoltAnimationManager::PlayAnimationFor(TScriptInterface<IVoltInterface> TargetVoltInterface,
                                                            const UVoltAnimation* Animation)
{
	if (Animation == nullptr)
	{
#if WITH_EDITOR
		UE_LOG(LogTemp, Error, TEXT("An empty animation has been requested to be played. Reverted the action."));
#endif
		return FVoltAnimationTrack::NullTrack; // Revert of there is no animation to play.
	}

	if (TargetVoltInterface == nullptr || !TargetVoltInterface.GetInterface())
	{
#if WITH_EDITOR
		UE_LOG(LogTemp, Error,
		       TEXT("An invalid slate interface has been requested to play an animation for. Reverted the action."));
#endif
		return FVoltAnimationTrack::NullTrack; // Revert of there is no animation to play.
	}

	const FVoltAnimationTrack& Track = MakeTrackWith(TargetVoltInterface, Animation);

	EnqueueOnAddAnimationTrack(Track);

	return Track;
}

FVoltAnimationTrack UVoltAnimationManager::MakeTrackWith(TScriptInterface<IVoltInterface> VoltInterface,
                                                         const UVoltAnimation* Animation)
{
	if (Animation == nullptr || VoltInterface == nullptr)
	{
		return FVoltAnimationTrack::NullTrack;
	}

	UVoltAnimation* AnimationInstance = DuplicateObject<UVoltAnimation>(Animation, this);

	if (!AnimationInstance)
	{
		return FVoltAnimationTrack::NullTrack;
	}

	for (UVoltModuleItem* Module : AnimationInstance->Modules)
	{
		if (!Module) continue;

		Module->SetVoltSlate(VoltInterface);
	}

	return FVoltAnimationTrack(VoltInterface, AnimationInstance);
}

void UVoltAnimationManager::EnqueueOnAddAnimationTrack(const FVoltAnimationTrack& Track)
{
	if (!AddAnimationTrackQueue.Contains(Track))
	{
		AddAnimationTrackQueue.Add(Track);
	}
}

void UVoltAnimationManager::EnqueueOnDeleteAnimationTrack(const FVoltAnimationTrack& Track)
{
	if (!DeleteAnimationTrackQueue.Contains(Track))
	{
		DeleteAnimationTrackQueue.Add(Track);
	}
}

void UVoltAnimationManager::DequeueOnAddAnimationTrack(const FVoltAnimationTrack& Track)
{
	AddAnimationTrackQueue.Remove(Track);
}

void UVoltAnimationManager::DequeueOnDeleteAnimationTrack(const FVoltAnimationTrack& Track)
{
	DeleteAnimationTrackQueue.Remove(Track);
}

void UVoltAnimationManager::ApplyQueuedAnimationTrackRequests()
{
	for (FVoltAnimationTrack& DeletionQueue : DeleteAnimationTrackQueue)
	{
		ProcessDeleteAnimationTrack(DeletionQueue);
	}
	
	for (FVoltAnimationTrack& AdditionQueue : AddAnimationTrackQueue)
	{
		ProcessAddAnimationTrack(AdditionQueue);
	}

	AddAnimationTrackQueue.Empty();
	DeleteAnimationTrackQueue.Empty();
}

void UVoltAnimationManager::ProcessAddAnimationTrack(FVoltAnimationTrack& Track)
{
	if (OnTrackAdded.IsBound())
	{
		OnTrackAdded.Broadcast(this, Track);
	}

	//Broadcast the event.
	if (OnAnimationPlayed.IsBound())
	{
		OnAnimationPlayed.Broadcast(this, Track, Track.TargetAnimation && Track.TargetAnimation.IsValid() ? Track.TargetAnimation.Get() : nullptr);
	}

	AnimationTracks.Add(Track);
	
}

void UVoltAnimationManager::ProcessDeleteAnimationTrack(FVoltAnimationTrack& Track)
{
	//Broadcast the event.
	if (OnAnimationEnded.IsBound())
	{
		OnAnimationEnded.Broadcast(this, Track, Track.TargetAnimation && Track.TargetAnimation.IsValid() ? Track.TargetAnimation.Get() : nullptr);
	}
	//Broadcast the event.
	if (OnTrackRemoved.IsBound())
	{
		OnTrackRemoved.Broadcast(this, Track);
	}
	
	//Release its animation.
	if (Track.TargetAnimation)
	{
		Track.TargetAnimation = nullptr;
	}

	AnimationTracks.Remove(Track);
}


void UVoltAnimationManager::FlushUnnecessaryTrack()
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
		}
	}
}


bool UVoltAnimationManager::HasTrack(const FVoltAnimationTrack& Track)
{
	const int FoundIndex = AnimationTracks.Find(Track);

	//Revert if it was unable to find the index of the target.
	if (FoundIndex == INDEX_NONE)
	{
		return false;
	}

	return true;
}

void UVoltAnimationManager::FlushTrack(const FVoltAnimationTrack& Track)
{
	//If add queue has this one, remove as well.
	DequeueOnAddAnimationTrack(Track);
	
	//Book that up for the deletion.
	EnqueueOnDeleteAnimationTrack(Track);
}

void UVoltAnimationManager::FlushTrackAt(const int Index)
{
	if (!AnimationTracks.IsValidIndex(Index))
	{
		return;
	}

	//Get the reference of the track from the array and use it instead of the provided track to avoid corruption.
	FVoltAnimationTrack& TrackRefToRemove = AnimationTracks[Index];

	FlushTrack(TrackRefToRemove);
}

void UVoltAnimationManager::FlushTracksFor(TScriptInterface<IVoltInterface> TargetVoltInterface)
{
	const int num = AnimationTracks.Num();

	//Iterate the array backward.
	for (int index = num - 1; index >= 0; --index)
	{
		if(!AnimationTracks.IsValidIndex(index)) continue;

		if (FVoltAnimationTrack& Track = AnimationTracks[index]; Track.TargetSlateInterface.GetInterface() == TargetVoltInterface.GetInterface())
		{
			FlushTrack(AnimationTracks[index]);
		}
	}
}


void UVoltAnimationManager::FlushAllTracks()
{
	const int num = AnimationTracks.Num();

	for (int index = num - 1; index >= 0; --index)
	{
		FlushTrackAt(index);
	}
}

const TArray<FVoltAnimationTrack>& UVoltAnimationManager::GetAnimationTracks()
{
	return AnimationTracks;
}

bool UVoltAnimationManager::IsPlayingAnimation()
{
	return !AnimationTracks.IsEmpty();
}

void UVoltAnimationManager::Tick(float DeltaTime)
{
	const UVoltSubsystem* Subsystem = UVoltSubsystem::Get();

	if (!Subsystem) return;
	
	// Manual update - for the case whether this object can not rely on external updates (thread)
	if (!Subsystem->IsUtilizingMultithreading())
	{
		ApplyQueuedAnimationTrackRequests();

		if(!AnimationTracks.IsEmpty()) ProcessModuleUpdate(DeltaTime);
	}

	ApplyVariables();

	// TODO: Provide better way to flush it down, and make sure to this process is not making any bottleneck
	if (!Subsystem->IsModuleUpdateThreadWorking())
	{
		FlushUnnecessaryTrack();
	}

}


void UVoltAnimationManager::ApplyVariables()
{
	for (FVoltAnimationTrack& AnimationTrack : AnimationTracks)
	{
		if (UVoltVariableCollection* Collection = AnimationTrack.TargetSlateInterface->GetVoltVariableCollection())
		{
			//Process all the queued variable to the actual list.
			Collection->ProcessQueue();
			
			const TArray<UVoltVariableBase*>& Variables = Collection->GetVariables();
				
			const int IterNum = Variables.Num();

			if(!AnimationTrack.TargetSlateInterface) continue;
			
			TWeakPtr<SWidget> Slate = AnimationTrack.TargetSlateInterface->GetTargetSlate();

			if(!Slate.IsValid()) continue;

			for (int i = 0; i < IterNum; ++i)
			{
				if (!Variables.IsValidIndex(i)) continue;

				if(UVoltVariableBase* Variable = Variables[i])
				{
					Variable->ApplyVariable(Slate);
				}
			}
			
		}
		
	}
}

const TSet<FVoltInterfaceElement> UVoltAnimationManager::GetVoltInterfacesBeingAnimated()
{
	TSet<FVoltInterfaceElement> Set;

	for (FVoltAnimationTrack& AnimationTrack : AnimationTracks)
	{
		Set.Add(AnimationTrack.TargetSlateInterface);
	}

	return Set;
}


void UVoltAnimationManager::ProcessModuleUpdate(float DeltaTime)
{
	for (FVoltAnimationTrack& AnimationTrack : AnimationTracks)
	{
		if (!AnimationTrack.TargetAnimation)
		{
			continue;
		}

		if (!AnimationTrack.TargetSlateInterface)
		{
			continue;
		}

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

			if (!Module->IsBegunPlay())
			{
				Module->BeginPlayModule();
			}
			
			if(Module->IsEndedPlay()) continue;

			if (!Module->IsActive())
			{
				Module->EndPlayModule();
				continue;
			}

			Module->ModifySlateVariable(DeltaTime, AnimationTrack.TargetSlateInterface);
		}
	}
}


void UVoltAnimationManager::ReleaseAll()
{
	//GC will handle the leftover for it.

	FlushAllTracks();

	SetOwnerVoltInterface(nullptr);
}

void UVoltAnimationManager::SetOwnerVoltInterface(const TScriptInterface<IVoltInterface> NewOwnerVoltInterface)
{
	OwnerVoltInterface = NewOwnerVoltInterface;
}

bool UVoltAnimationManager::CheckShouldDestruct() const
{
	if (const UVoltSubsystem* Subsystem = UVoltSubsystem::Get(); GetOuter() && GetOuter() != Subsystem)
	{
		return false;
	}

	if (OwnerVoltInterface->GetTargetSlate().IsValid())
	{
		return false;
	}

	if (IsRooted())
	{
		return false;
	}

	return true;
}

void UVoltAnimationManager::DestructSelf()
{
	ReleaseAll();

	MarkAsGarbage();
}
