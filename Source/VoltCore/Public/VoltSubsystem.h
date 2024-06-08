// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/EngineSubsystem.h"
#include "Tickable.h"
#include "VoltSubsystem.generated.h"

class UVoltAnimationManager;
/**
 * A subsystem for Volt.
 * It handles some of the fundamental and essential logic for the system.
 */
UCLASS()
class VOLTCORE_API UVoltSubsystem : public UEngineSubsystem, public FTickableGameObject
{
	GENERATED_BODY()

public:
	
	UVoltSubsystem();

	~UVoltSubsystem();

public:
	
	/**
	 * Register an animation manager on the subsystem.
	 * This action will prevent the animation manager to be GC'd.
	 * @param AnimationManager The animation manager to register.
	 */
	void RegisterAnimationManager(UVoltAnimationManager* AnimationManager);

	/**
	 * Unregister an animation manager on the subsystem.
	 * This action will prevent the animation manager to be GC'd.
	 * @param AnimationManager The animation manager to unregister.
	 */
	void UnregisterAnimationManager(UVoltAnimationManager* AnimationManager);
	
private:

	/**
	 * An array of the active animation manager instances.
	 * Any animation managers must be assigned on this array to prevent GC.
	 */
	UPROPERTY()
	TArray<UVoltAnimationManager*> RegisteredAnimationManager;

public:

	void UpdateAnimations(float DeltaTime);

	void DestructUnnecessaryRegisteredAnimationManager();

public:

	//World reference related
	
	/**
	 * Get the world when it has been opened.
	 * @param World New world instance;
	 */
	void OnPostLoadMapWithWorld(UWorld* World);

	virtual UWorld* GetWorld() const override;

private:

	UPROPERTY()
	TWeakObjectPtr<UWorld> WorldReference;

private:

	//Tick related.
	
	virtual void Tick(float DeltaTime) override;

	virtual bool IsTickable() const override;

	virtual ETickableTickType GetTickableTickType() const override;

	virtual TStatId GetStatId() const override;

	virtual bool IsTickableInEditor() const override;

	virtual bool IsTickableWhenPaused() const override;

	virtual UWorld* GetTickableGameObjectWorld() const override;

public:

	/**
	 * Get the singleton instance of Volt Subsystem. 
	 * @return The singleton instance of Volt Subsystem.
	 */
	static UVoltSubsystem* Get();
};
