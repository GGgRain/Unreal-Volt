//Copyright 2022~2024 DevGrain. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "VoltEditorSettings.generated.h"


UCLASS(config = VoltEditorSetting, Defaultconfig)
class VOLTEDITOR_API UVoltEditorSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	
	UVoltEditorSettings();

	~UVoltEditorSettings();


public:

	virtual void PostLoad() override;
	
public:
	
	/**
	 * Check whether the setting has been initialized. 
	 * @return whether the setting has been initialized. 
	 */
	UFUNCTION()
	const bool CheckEverInitialized();

	/**
	 * Mark the setting as initialized.
	 */
	UFUNCTION()
	void MarkAsInitialized();

public:
	
	/**
	 * Reset the module's setting to the default.
	 */
	UFUNCTION(CallInEditor, Category="Volt Setting")
	void ResetSetting();
	
	UFUNCTION()
	void ResetPerformanceSettings(const bool bSaveConfig);

private:
	
	/**
	 * Internal flag for the save action.
	 */
	UPROPERTY(config)
	bool bEverInitialized = false;

private:

	/**
	 * Reset runtime settings to the default settings.
	 * When it has been selected, it will pop up a dialogue about the reset action.
	 * Adding a whole editor module only for the reset action was a kinda overkill for the performance, and this is a compromised option for it.
	 */
	UPROPERTY(config, EditAnywhere, Category="Volt Settings",DisplayName="Reset Runtime Settings To Default")
	bool bReset = false;

private:
	
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

private:

	void RuntimeSettingPostEditChangeProperty(UObject* Obj, FPropertyChangedEvent& PropertyChangedEvent);

	void RegisterOnRuntimeSettingPostEditChangeProperty();

	void UnregisterOnRuntimeSettingPostEditChangeProperty();

public:
	
	virtual bool IsEditorOnly() const override;
	virtual FName GetCategoryName() const override final { return TEXT("Volt"); }
	virtual FText GetSectionText() const override final { return FText::FromString("Volt Editor Preferences"); }
	
public:
	
	//Singleton instance

	static UVoltEditorSettings* Get();
	
};
