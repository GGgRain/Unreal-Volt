//Copyright 2022~2024 DevGrain. All Rights Reserved.


#include "VoltEditorSettings.h"

#include "ISettingsEditorModule.h"
#include "VoltSettings.h"
#include "HAL/PlatformFileManager.h"

#include "VariableActions/VoltVariableActions.h"
#include "Variables/VoltVariables.h"

#define MACRO_SAVE_RUNTIME_CONFIG() \
{ \
	{ \
		if(UVoltSettings* Setting = UVoltSettings::Get()) { Setting->SaveConfig(CPF_Config, *Setting->GetDefaultConfigFilename()); } \
	} \
}



UVoltEditorSettings::UVoltEditorSettings()
{
	RegisterOnRuntimeSettingPostEditChangeProperty();
}

UVoltEditorSettings::~UVoltEditorSettings()
{
	UnregisterOnRuntimeSettingPostEditChangeProperty();
}

void UVoltEditorSettings::PostLoad()
{
	Super::PostLoad();

	if(!CheckEverInitialized()) ResetSetting();

}

void UVoltEditorSettings::ResetSetting()
{
	
	ResetPerformanceSettings(false);

	MACRO_SAVE_RUNTIME_CONFIG()

	MarkAsInitialized();
	
}

void UVoltEditorSettings::ResetPerformanceSettings(const bool bSaveConfig)
{
	if(UVoltSettings* Setting = UVoltSettings::Get())
	{
		Setting->bUseMultithreadingOnModuleUpdate = true;
		Setting->AnimationManagerCleanUpInterval = 37;
	}
	if(bSaveConfig) MACRO_SAVE_RUNTIME_CONFIG()
	
	MarkAsInitialized();
}

#include "Widgets/Notifications/SNotificationList.h"
#include "Framework/Notifications/NotificationManager.h"
#include "Misc/MessageDialog.h"

#define LOCTEXT_NAMESPACE "VoltSettings"

void UVoltEditorSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if(bReset)
	{
		EAppReturnType::Type Type = FMessageDialog::Open( EAppMsgType::OkCancel,LOCTEXT("ResetDialog", "Reset action cannot be reverted. Are you sure to proceed?"));

		if(Type == EAppReturnType::Ok)
		{
			FNotificationInfo NotificationInfo(LOCTEXT("ResetDialog_Confirm", "All properties has been reset."));
			NotificationInfo.bFireAndForget = true;
			NotificationInfo.FadeInDuration = 0.3f;
			NotificationInfo.FadeOutDuration = 1.3f;
			NotificationInfo.ExpireDuration = 4.5f;
		
			FSlateNotificationManager::Get().AddNotification(NotificationInfo);

			ResetSetting();
		}
	
		bReset = false;
	}

	MarkAsInitialized();
}

void UVoltEditorSettings::RuntimeSettingPostEditChangeProperty(UObject* Obj, FPropertyChangedEvent& PropertyChangedEvent)
{
	if (ISettingsEditorModule* SettingsEditorModule = FModuleManager::GetModulePtr<ISettingsEditorModule>("SettingsEditor")) SettingsEditorModule->OnApplicationRestartRequired();

	MACRO_SAVE_RUNTIME_CONFIG()

	MarkAsInitialized();
}

void UVoltEditorSettings::RegisterOnRuntimeSettingPostEditChangeProperty()
{
	if(UVoltSettings* Setting = UVoltSettings::Get()) Setting->OnSettingChanged().AddUObject(this, &UVoltEditorSettings::RuntimeSettingPostEditChangeProperty);
}

void UVoltEditorSettings::UnregisterOnRuntimeSettingPostEditChangeProperty()
{
	if(UVoltSettings* Setting = UVoltSettings::Get()) Setting->OnSettingChanged().RemoveAll(this);
}

bool UVoltEditorSettings::IsEditorOnly() const
{
	return true;
}

UVoltEditorSettings* UVoltEditorSettings::Get()
{
	UVoltEditorSettings* ProxyDevSettings = CastChecked<UVoltEditorSettings>(UVoltEditorSettings::StaticClass()->GetDefaultObject());
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	return ProxyDevSettings;
}


#undef LOCTEXT_NAMESPACE

const bool UVoltEditorSettings::CheckEverInitialized()
{
	return bEverInitialized;
}

void UVoltEditorSettings::MarkAsInitialized()
{
	// if it has been marked as initialized already, fall back.
	if(bEverInitialized) return;

	bEverInitialized = true;

	SaveConfig(CPF_Config, *GetDefaultConfigFilename());
}

#undef MACRO_REGISTER_VARIABLE_ACTION_ON_RUNTIME_CONFIG
#undef MACRO_CLEAR_VARIABLE_ACTION_ON_RUNTIME_CONFIG
#undef MACRO_SAVE_RUNTIME_CONFIG
