//Copyright 2022~2024 DevGrain. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "VoltModuleItem.generated.h"

class IVoltInterface;
class SWidget;

/*
 * A base object class that handles modification of variables as intended.
 * It is inspired by the niagara stack modules' structure and designed to perform as same as that.
 * Override this class to define custom actions for the variable modification.
 */
UCLASS(EditInlineNew, Blueprintable, BlueprintType, Abstract)
class VOLTCORE_API UVoltModuleItem : public UObject
{
	GENERATED_BODY()

public:

	/**
	 * Modify the slate's variable. This function will be triggered every frame. (equivalent of Tick())
	 * @param DeltaTime Delta time from the last frame
	 * @param Volt Volt interface to animated
	 */
	UFUNCTION(BlueprintCallable, Category="Module Action")
	virtual void ModifySlateVariable(const float DeltaTime, const TScriptInterface<IVoltInterface>& Volt);

	UFUNCTION(BlueprintImplementableEvent, Category="Module Action", meta = (DisplayName = "Modify Slate Variable"))
	void K2_ModifySlateVariable(const float DeltaTime, const TScriptInterface<IVoltInterface>& Volt);

public:
	
	/**
	 * Get volt slate that this module is modifying.
	 * @return get the volt slate (volt interface) that this module is affecting.
	 */
	UFUNCTION(BlueprintCallable, Category="Module Action")
	const TScriptInterface<IVoltInterface>& GetVoltSlate();
	
	/**
	 * Set volt slate that this module is modifying.
	 * @param Slate the volt slate (volt interface) that this module will affect.
	 */
	UFUNCTION(BlueprintCallable, Category="Module Action")
	void SetVoltSlate(const TScriptInterface<IVoltInterface>& Slate);

public:
	
	
	/**
	 * Whether the module should continue to work and modification of the slate variables.
	 * @return false when this nodule's action is finished.
	 */
	UFUNCTION(BlueprintCallable, Category="Module Life Cycle")
	virtual bool IsActive();

	//Return false when this nodule's action is finished.
	UFUNCTION(BlueprintImplementableEvent, Category="Module Action", meta = (DisplayName = "Is Active"))
	bool K2_IsActive();

public:
	
	UPROPERTY(Transient)
	TScriptInterface<IVoltInterface> TargetVoltSlate;
	
	//transient
	TWeakPtr<SWidget> TargetSlate;
	
};
