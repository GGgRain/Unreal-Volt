//Copyright 2022~2024 DevGrain. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "VoltModuleItem.h"
#include "Layout/Margin.h"
#include "Volt_ASM_Sequence.generated.h"

/**
 * A module for the sequence animation playback.
 * This class is highly experimental.
 */
UCLASS(EditInlineNew, Blueprintable, BlueprintType)
class VOLT_API UVolt_ASM_Sequence : public UVoltModuleItem
{
	
	GENERATED_BODY()

public:
	
	virtual void ModifySlateVariable(const float DeltaTime, const TScriptInterface<IVoltInterface>& Volt) override;

	virtual bool IsActive() override;

public:
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Module Property")
	TArray<UVoltModuleItem*> Modules;

public:
	
	void PickupNextModule();

public:

	template <class AnimClass>
	AnimClass* GetModuleAt(int Index);
	
	template <class AnimClass>
	AnimClass* GetModuleForClass();
	
public:

	UPROPERTY()
	int CurrentlyPlayingModuleIdx = 0;

public:

	/**
	 * Whether to loop the playback.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Module Property")
	bool bShouldLoop = false;

private:
	
	UPROPERTY(Transient)
	bool bEverUpdated = false; //If not specified, do nothing.
	
};

template <typename AnimClass>
AnimClass* UVolt_ASM_Sequence::GetModuleAt(const int Index)
{
	
	UVoltModuleItem* FoundModule = nullptr;
	
	if (Modules.IsValidIndex(Index)) FoundModule = Modules[Index];
	
	return FoundModule ? Cast<AnimClass>(FoundModule) : nullptr;
}


template <typename AnimClass>
AnimClass* UVolt_ASM_Sequence::GetModuleForClass()
{
	
	UVoltModuleItem* FoundModule = nullptr;
	
	if (Modules.IsEmpty()) return nullptr; // Inactive since there is no module to play.

	for (UVoltModuleItem* Module : Modules)
	{
		if (Module->GetClass() == AnimClass::StaticClass())
		{
			FoundModule = Module;
			
			break;
		}
	}
	
	return FoundModule ? Cast<AnimClass>(FoundModule) : nullptr;
}