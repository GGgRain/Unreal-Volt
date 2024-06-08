//Copyright 2022~2024 DevGrain. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "VoltAnimationTrack.h"
#include "VoltInterface.h"
#include "VoltSubsystem.h"
#include "Misc/Attribute.h"
#include "Templates/SubclassOf.h"
#include "Engine/Engine.h"
#include "VoltAnimationManager.generated.h"

class UVoltSubsystem;
class UVoltProxy;
class UVoltAnimation;
class UVoltAnimationManager;

/**
 * Any animation that works on Volt will not be played on the debug mode of the editor & slate application by default.
 * This class is not belong to the slate application architecture, and this is the limitation of the engine, because they hard-coded the refresh action of the slates in while-loop while not triggering any other engine tick events.
 * But we can still make it work on the debug state by executing tick action manually on the slate's tick event.
 *
 * For example, we can do something like this:
 *
 * void SContextTextStyler::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
 * {
 *		if(!UVoltAnimationManager::CheckCanTickAutomatically()) VoltAnimManager->Tick(InDeltaTime);
 *		
 *		SCompoundWidget::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);
 *	}
 */

/**
 * Volt animation manager uses its unique way to be marked as garbage by itself.
 * 
 * Every Volt animation manager will be self-destructed if it doesn't fulfill any of the requirements.
 * 
 * The requirements that the animation manager requires is like as following:
 * 1. If it has any specific outer other than the VoltSubsystem, then it will ignore the self-destruction.
 * 2. If it has any valid slates assigned on the animation manager, then it will ignore the self-destruction.
 *
 * So, you have to assign at least one valid slate to make it persistent.
 * If you are going to use Volt on animating a layout slate, then we recommend to assign the layout slate itself to prevent GC.
 * Like this:
 * 
 *  VOLT_IMPLEMENT_MANAGER(&AnimationManager);
 *	//To prevent GC.
 *	AnimationManager->AssignSlate(this->AsShared());
 *	
 */




DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnAnimationPlayed,
                                               UVoltAnimationManager*, Manager,
                                               const FVoltAnimationTrack&, Track,
                                               const UVoltAnimation*, Animation);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnAnimationEnded,
                                               UVoltAnimationManager*, Manager,
                                               const FVoltAnimationTrack&, Track,
                                               const UVoltAnimation*, Animation);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTrackAdded,
                                             UVoltAnimationManager*, Manager,
                                             const FVoltAnimationTrack&, Track);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTrackRemoved,
                                             UVoltAnimationManager*, Manager,
                                             const FVoltAnimationTrack&, Track);

UCLASS()
class VOLTCORE_API UVoltAnimationManager : public UObject
{
	GENERATED_BODY()

public:
	//Animation related.

	//Play a certain animation to the all characters widget. The widget must be assigned to this instance.
	UFUNCTION(BlueprintCallable, Category="Animation")
	TArray<FVoltAnimationTrack> PlayAnimationForAll(const UVoltAnimation* Animation);

	//Play a certain animation to specific slate on the animated slate interface. The widget must be assigned to this instance.
	UFUNCTION(BlueprintCallable, Category="Animation")
	FVoltAnimationTrack PlayAnimationFor(TScriptInterface<IVoltInterface> TargetSlateInterface,
	                                     const UVoltAnimation* Animation);

	//Play a certain animation to specific widget for the provided index. The widget must be assigned to this instance.
	UFUNCTION(BlueprintCallable, Category="Animation")
	FVoltAnimationTrack PlayAnimationAt(const int& Index, const UVoltAnimation* Animation);

	//Update all animations of all children slates.
	UFUNCTION(BlueprintCallable, Category="Animation")
	virtual void UpdateAnimations(float DeltaTime);

	/**
	 * Tick animation and update the slates. This function can be triggered manually.
	 * @param DeltaTime Delta time from the last update.
	 */
	UFUNCTION(BlueprintCallable, Category="Animation")
	void TickAnimations(float DeltaTime);

	/**
	 * Flush all the unnecessary animation.
	 */
	UFUNCTION(BlueprintCallable, Category="Animation")
	void FlushUnnecessaryAnimation();


	//Flush specific track.
	UFUNCTION(BlueprintCallable, Category="Animation")
	bool HasTrack(const FVoltAnimationTrack& Track);

	//Flush specific track.
	UFUNCTION(BlueprintCallable, Category="Animation")
	void FlushTrack(const FVoltAnimationTrack& Track);

	//Flush the track at the provided index.
	UFUNCTION(BlueprintCallable, Category="Animation")
	void FlushTrackAt(const int Index);


	//Flush all the animation tracks for specific slate.
	UFUNCTION(BlueprintCallable, Category="Animation")
	void FlushTracksFor(TScriptInterface<IVoltInterface> TargetSlateInterface);

	//Flush all the tracks.
	UFUNCTION(BlueprintCallable, Category="Animation")
	void FlushAllTracks();

	//Get the whole animation tracks
	UFUNCTION(BlueprintCallable, Category="Animation")
	const TArray<FVoltAnimationTrack>& GetAnimationTracks();

	//Get the whole slates that are assigned to this node.
	UFUNCTION(BlueprintCallable, Category="Animation")
	const TArray<TScriptInterface<IVoltInterface>>& GetVolts();

public:
	//Slate interface related.

	//Assign the interface as its child.
	UFUNCTION(BlueprintCallable, Category="Slate")
	TScriptInterface<IVoltInterface> AssignSlate(TScriptInterface<IVoltInterface> SlateToAdd);

	//Create a proxy for the provided slate and assign it as its child. Return the created proxy.
	TScriptInterface<IVoltInterface> AssignSlate(const TWeakPtr<SWidget>& SlateToAdd);

	TScriptInterface<IVoltInterface> FindSlateFor(
		const TScriptInterface<IVoltInterface>& SlateInterfaceToCheck) const;

	TScriptInterface<IVoltInterface> FindSlateFor(const TWeakPtr<SWidget>& SlateToFind) const;

	TScriptInterface<IVoltInterface> FindOrAssignSlate(
		const TScriptInterface<IVoltInterface>& SlateInterfaceToCheck);

	TScriptInterface<IVoltInterface> FindOrAssignSlate(const TWeakPtr<SWidget>& SlateToFind);


	bool CheckHasSlate(const TScriptInterface<IVoltInterface>& SlateInterfaceToCheck) const;

	bool CheckHasSlate(const TWeakPtr<SWidget>& SlateToCheck) const;

	int GetSlateCount() const;

public:
	//Return whether this manager is playing any animation.
	bool IsPlayingAnimation();

	//Return whether the specific slate is playing any animation on this manager.
	bool CheckSlatePlayingAnimation(const TScriptInterface<IVoltInterface>& SlateInterfaceToCheck);

	//Return whether the specific slate is playing any animation on this manager.
	bool CheckSlatePlayingAnimation(const TWeakPtr<SWidget>& SlateToCheck);

public:
	
	/**
	 * Tick the object. This will update the slates' animation.
	 * Execute this function to manually trigger animation update.
	 */
	void Tick(float DeltaTime);
	
public:

	
	//Release the whole animation and slate data.
	UFUNCTION(BlueprintCallable, Category="Control")
	void ReleaseAll();
	
private:

	/**
	 * Check whether this animation manager fulfill any of the requirements so can ignore the self-destruction.
	 * Every Volt animation manager will be self-destructed if it doesn't fulfill any of the requirements.
	 * 
	 * The requirements that the animation manager requires is like as following:
	 * 1. If it has any specific outer other than the VoltSubsystem, then it will ignore the self-destruction.
	 * 2. If it has any valid slates assigned on the animation manager, then it will ignore the self-destruction.
	 */
	bool CheckShouldDestruct() const;

	/**
	 * self-destruct the object instance.
	 * This class NEVER call this function by itself.
	 */
	void DestructSelf();

	friend UVoltSubsystem;

public:
	
	virtual UWorld* GetWorld() const override;
	
private:
	//Animated slates that are involved as children to this instance.
	UPROPERTY(VisibleAnywhere, Category="Animated Slates", SkipSerialization, DuplicateTransient, Transient)
	TArray<TScriptInterface<IVoltInterface>> Volts;

	//Animations for the slates.
	UPROPERTY(VisibleAnywhere, Category="Animated Slates", SkipSerialization, DuplicateTransient, Transient)
	TArray<FVoltAnimationTrack> AnimationTracks;

public:
	UPROPERTY(BlueprintAssignable, Category="Manager Action")
	FOnAnimationPlayed OnAnimationPlayed;

	UPROPERTY(BlueprintAssignable, Category="Manager Action")
	FOnAnimationEnded OnAnimationEnded;

	UPROPERTY(BlueprintAssignable, Category="Manager Action")
	FOnTrackAdded OnTrackAdded;

	UPROPERTY(BlueprintAssignable, Category="Manager Action")
	FOnTrackRemoved OnTrackRemoved;

private:
	
	/**
	 * Whether to use manual tick for this instance. If this value is true, you have to call void Tick(float DeltaTime) manually for this instance to animate the slates.
	 * This is useful when you have to use this animation manager work on some rare occasions such as while a drag-drop event is active or SlateApplication is on debug state.
	 *
	 * This variable and following features are highly experimental. Can be replaced if we find a better way.
	 */
	UPROPERTY(BlueprintGetter=CheckUseManualTick, BlueprintSetter=SetUseManualTick, Category="Tick")
	bool bUseManualTick = false;

public:

	/**
	 * Check whether this animation manager requires to be ticked manually by users.
	 * @return whether this animation manager must be ticked manually. 
	 */
	UFUNCTION(BlueprintPure, BlueprintCallable, Category="Tick")
	bool CheckUseManualTick();

	
	/**
	 * Set whether this animation manager must be ticked manually or not.
	 * @param NewUseManualTick whether this animation manager will be ticked manually or not.
	 */
	UFUNCTION(BlueprintCallable, Category="Tick")
	void SetUseManualTick(bool NewUseManualTick);
	
};

/**
 * Implement a new animation manager.
 * @param ManagerPtr A pointer to the property that will store new animation manager object.
 * @param Outer An outer of the new animation manager. If the outer has world reference, new animation manager will use that reference to animate slates. If it was nullptr, then the Volt subsystem will be used as its outer.
 */
FORCEINLINE void VOLTCORE_API VOLT_IMPLEMENT_MANAGER(
	TObjectPtr<UVoltAnimationManager>* ManagerPtr, UObject* Outer = nullptr)
{
	UVoltAnimationManager* NewManager = nullptr;

	if(GEngine == nullptr) return;
	
	UEngineSubsystem* Subsystem = GEngine->GetEngineSubsystemBase(UVoltSubsystem::StaticClass());

	//If the subsystem is not initialized, revert the action.
	if (Subsystem == nullptr) return;

	UVoltSubsystem* CastedSubsystem = Cast<UVoltSubsystem>(Subsystem);

	if (CastedSubsystem == nullptr) return;
	
	if (Outer != nullptr)
	{
		if (Outer->IsValidLowLevel())
		{
			NewManager = NewObject<UVoltAnimationManager>(Outer);
		}
	}

	if (NewManager == nullptr)
	{
		NewManager = NewObject<UVoltAnimationManager>(Subsystem);
	}
	
	if (NewManager != nullptr)
	{
		CastedSubsystem->RegisterAnimationManager(NewManager);
		(*ManagerPtr) = NewManager;
	}
}


/**
 * Implement a new animation manager. This function is not recommended to be executed by users.
 * @param ManagerPtr A pointer to the property that will store new animation manager object.
 * @param Outer An outer of the new animation manager. If the outer has world reference, new animation manager will use that reference to animate slates. If it was nullptr, then the Volt subsystem will be used as its outer.
 */
FORCEINLINE void VOLTCORE_API VOLT_IMPLEMENT_MANAGER(
	UVoltAnimationManager** ManagerPtr, UObject* Outer = nullptr)
{
	UVoltAnimationManager* NewManager = nullptr;

	if(GEngine == nullptr) return;

	UEngineSubsystem* Subsystem = GEngine->GetEngineSubsystemBase(UVoltSubsystem::StaticClass());

	//If the subsystem is not initialized, revert the action.
	if (Subsystem == nullptr) return;

	UVoltSubsystem* CastedSubsystem = Cast<UVoltSubsystem>(Subsystem);

	if (CastedSubsystem == nullptr) return;
	
	if (Outer != nullptr)
	{
		if (Outer->IsValidLowLevel())
		{
			NewManager = NewObject<UVoltAnimationManager>(Outer);
		}
	}

	if (NewManager == nullptr)
	{
		NewManager = NewObject<UVoltAnimationManager>(Subsystem);
	}
	
	if (NewManager != nullptr)
	{
		CastedSubsystem->RegisterAnimationManager(NewManager);
		(*ManagerPtr) = NewManager;
	}
}

FORCEINLINE void VOLTCORE_API VOLT_RELEASE_MANAGER(
	UVoltAnimationManager** ManagerPtr)
{
	if(GEngine == nullptr) return;

	UEngineSubsystem* Subsystem = GEngine->GetEngineSubsystemBase(UVoltSubsystem::StaticClass());

	//If the subsystem is not initialized, revert the action.
	if (Subsystem == nullptr) return;

	UVoltSubsystem* CastedSubsystem = Cast<UVoltSubsystem>(Subsystem);

	if (CastedSubsystem == nullptr) return;
	
	if ((*ManagerPtr) == nullptr) return;
	if (!(*ManagerPtr)->IsValidLowLevel()) return;

	CastedSubsystem->UnregisterAnimationManager(*ManagerPtr);

	(*ManagerPtr) = nullptr;
}


FORCEINLINE void VOLTCORE_API VOLT_RELEASE_MANAGER(
	TObjectPtr<UVoltAnimationManager>* ManagerPtr)
{
	if(GEngine == nullptr) return;

	UEngineSubsystem* Subsystem = GEngine->GetEngineSubsystemBase(UVoltSubsystem::StaticClass());

	//If the subsystem is not initialized, revert the action.
	if (Subsystem == nullptr) return;

	UVoltSubsystem* CastedSubsystem = Cast<UVoltSubsystem>(Subsystem);

	if (CastedSubsystem == nullptr) return;
	
	if ((*ManagerPtr) == nullptr) return;
	if (!(*ManagerPtr)->IsValidLowLevel()) return;

	CastedSubsystem->UnregisterAnimationManager(*ManagerPtr);

	(*ManagerPtr) = nullptr;
}


FORCEINLINE TScriptInterface<IVoltInterface> VOLTCORE_API VOLT_FIND_OR_ASSIGN_SLATE(
	UVoltAnimationManager* Manager,
	const TWeakPtr<SWidget>& Slate)
{
	if (Manager == nullptr) return nullptr;
	if (!Manager->IsValidLowLevel()) return nullptr;
	if (Slate == nullptr) return nullptr;

	return Manager->FindOrAssignSlate(Slate);
}


FORCEINLINE TScriptInterface<IVoltInterface> VOLTCORE_API VOLT_FIND_OR_ASSIGN_SLATE(
	UVoltAnimationManager* Manager,
	TScriptInterface<IVoltInterface> SlateInterface)
{
	if (Manager == nullptr) return nullptr;
	if (!Manager->IsValidLowLevel()) return nullptr;
	if (SlateInterface == nullptr) return nullptr;

	return Manager->FindOrAssignSlate(SlateInterface);
}

FORCEINLINE TScriptInterface<IVoltInterface> VOLTCORE_API VOLT_ASSIGN_SLATE(
	UVoltAnimationManager* Manager,
	TScriptInterface<IVoltInterface> SlateInterface)
{
	if (Manager == nullptr) return nullptr;
	if (!Manager->IsValidLowLevel()) return nullptr;
	if (!SlateInterface) return nullptr;

	return Manager->AssignSlate(SlateInterface);
}


FORCEINLINE const FVoltAnimationTrack VOLTCORE_API VOLT_PLAY_ANIM(
	UVoltAnimationManager* Manager,
	const TWeakPtr<SWidget>& Slate,
	const UVoltAnimation* Animation)
{
	if (Manager == nullptr) return FVoltAnimationTrack::NullTrack;
	if (!Manager->IsValidLowLevel()) return FVoltAnimationTrack::NullTrack;
	if (Slate == nullptr) return FVoltAnimationTrack::NullTrack;
	if (Animation == nullptr) return FVoltAnimationTrack::NullTrack;


	return Manager->PlayAnimationFor(
		VOLT_FIND_OR_ASSIGN_SLATE(Manager, Slate),
		Animation);
}


FORCEINLINE const FVoltAnimationTrack VOLTCORE_API VOLT_PLAY_ANIM(
	UVoltAnimationManager* Manager,
	TScriptInterface<IVoltInterface> SlateInterface,
	const UVoltAnimation* Animation)
{
	if (Manager == nullptr) return FVoltAnimationTrack::NullTrack;
	if (!Manager->IsValidLowLevel()) return FVoltAnimationTrack::NullTrack;
	if (SlateInterface == nullptr) return FVoltAnimationTrack::NullTrack;
	if (Animation == nullptr) return FVoltAnimationTrack::NullTrack;


	return Manager->PlayAnimationFor(SlateInterface, Animation);
}

FORCEINLINE void VOLTCORE_API VOLT_STOP_ANIM(
	UVoltAnimationManager* Manager,
	const FVoltAnimationTrack& Track)
{
	if (Manager == nullptr) return;
	if (!Manager->IsValidLowLevel()) return;

	Manager->FlushTrack(Track);
}

FORCEINLINE bool VOLTCORE_API VOLT_CHECK_PLAYING_ANIM(
	UVoltAnimationManager* Manager,
	const FVoltAnimationTrack& Track)
{
	if (Manager == nullptr) return false;
	if (!Manager->IsValidLowLevel()) return false;

	return Manager->HasTrack(Track);
}


FORCEINLINE void VOLTCORE_API VOLT_STOP_ALL_ANIM(
	UVoltAnimationManager* Manager,
	TScriptInterface<IVoltInterface> SlateInterface)
{
	if (Manager == nullptr) return;
	if (!Manager->IsValidLowLevel()) return;
	if (SlateInterface == nullptr) return;

	Manager->FlushTracksFor(SlateInterface);
}

FORCEINLINE void VOLTCORE_API VOLT_STOP_ALL_ANIM(
	UVoltAnimationManager* Manager,
	const TWeakPtr<SWidget>& Slate)
{
	if (Manager == nullptr) return;
	if (!Manager->IsValidLowLevel()) return;
	if (Slate == nullptr) return;

	Manager->FlushTracksFor(VOLT_FIND_OR_ASSIGN_SLATE(Manager, Slate));
}
