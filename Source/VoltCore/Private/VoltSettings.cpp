// Fill out your copyright notice in the Description page of Project Settings.


#include "VoltSettings.h"


UVoltSettings::UVoltSettings()
{
	
}


UVoltSettings* UVoltSettings::Get()
{
	UVoltSettings* ProxyDevSettings = CastChecked<UVoltSettings>(UVoltSettings::StaticClass()->GetDefaultObject());
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	return ProxyDevSettings;
}
