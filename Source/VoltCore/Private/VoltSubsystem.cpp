// Fill out your copyright notice in the Description page of Project Settings.


#include "VoltSubsystem.h"
#include "VoltAnimationManager.h"
#include "Engine/Engine.h"


UVoltSubsystem::UVoltSubsystem()
{
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UVoltSubsystem::OnPostLoadMapWithWorld);
}

UVoltSubsystem::~UVoltSubsystem()
{
	FCoreUObjectDelegates::PostLoadMapWithWorld.RemoveAll(this);
}

void UVoltSubsystem::RegisterAnimationManager(UVoltAnimationManager* AnimationManager)
{
	if(!RegisteredAnimationManager.Contains(AnimationManager)) RegisteredAnimationManager.Add(AnimationManager);
}

void UVoltSubsystem::UnregisterAnimationManager(UVoltAnimationManager* AnimationManager)
{
	if(RegisteredAnimationManager.Contains(AnimationManager)) RegisteredAnimationManager.Remove(AnimationManager);
}

void UVoltSubsystem::UpdateAnimations(float DeltaTime)
{
	for (UVoltAnimationManager* AnimationManager : RegisteredAnimationManager)
	{
		if(AnimationManager == nullptr) continue;

		if(AnimationManager->CheckUseManualTick()) continue;
		
		AnimationManager->Tick(DeltaTime);
	}
}

void UVoltSubsystem::OnPostLoadMapWithWorld(UWorld* World)
{
	WorldReference = World;
}

UWorld* UVoltSubsystem::GetWorld() const
{
	if(WorldReference.IsValid()) return WorldReference.Get();
	
	return nullptr;
}


void UVoltSubsystem::DestructUnnecessaryRegisteredAnimationManager()
{
	RegisteredAnimationManager.RemoveAll([](UVoltAnimationManager* AnimationManager)
	{
		if(AnimationManager != nullptr)
		{
			if(!AnimationManager->CheckShouldDestruct()) return false;
		}

		AnimationManager->DestructSelf();

		return true;
	});
}

void UVoltSubsystem::Tick(float DeltaTime)
{
	//do animation things;
	UpdateAnimations(DeltaTime);

	//destruct all the unnecessary animation managers.
	DestructUnnecessaryRegisteredAnimationManager();
	
}

bool UVoltSubsystem::IsTickable() const
{
	return !RegisteredAnimationManager.IsEmpty();
}

ETickableTickType UVoltSubsystem::GetTickableTickType() const
{
	return ETickableTickType::Conditional;
}

bool UVoltSubsystem::IsTickableInEditor() const
{
	return true;
}

bool UVoltSubsystem::IsTickableWhenPaused() const
{
	return true;
}

UWorld* UVoltSubsystem::GetTickableGameObjectWorld() const
{
	return GetWorld();
}

UVoltSubsystem* UVoltSubsystem::Get()
{
	if(GEngine)
	{
		UEngineSubsystem* Subsystem = GEngine->GetEngineSubsystemBase(UVoltSubsystem::StaticClass());

		if(Subsystem)
		{
			return Cast<UVoltSubsystem>(Subsystem);
		}
	}
	
	return nullptr;
}

TStatId UVoltSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UVoltSubsystem, STATGROUP_Tickables);
}
