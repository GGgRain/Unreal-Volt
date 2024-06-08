//Copyright 2022~2024 DevGrain. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "UObject/NoExportTypes.h"
#include "VoltAnimation.generated.h"

class UVoltAnimation;
class UVoltModuleItem;

//Assign animation module to the target animation. Must be called in CDO and not elsewhere.
//#define VOLT_ANIMATION_ASSIGN_DEFAULT_MODULE(ModuleType, ModuleName)  Modules.Add_GetRef(CreateDefaultSubobject<ModuleType>(ModuleName))

/**
 * The animation asset class for the slates. It only contains data.
 */
UCLASS()
class VOLTCORE_API UVoltAnimation : public UObject
{
	GENERATED_BODY()
	
public:

	//Modules
	UPROPERTY(Instanced, EditAnywhere, Category="Animation")
	TArray<UVoltModuleItem*> Modules;

public:

	UFUNCTION(BlueprintCallable, Category="Animation", DisplayName="GetModuleForClass")
	UVoltModuleItem* K2_GetModuleForClass(const TSubclassOf<UVoltModuleItem> TargetModuleClass);
	
public:

	UFUNCTION(BlueprintCallable, Category="Animation")
	const bool IsActive() const;

public:

	//for C++ usage. 
	template<typename AnimClass=UVoltModuleItem>
	AnimClass* GetModuleForClass();
	
};

template <typename AnimClass>
AnimClass* UVoltAnimation::GetModuleForClass()
{

	UVoltModuleItem* Module = K2_GetModuleForClass(AnimClass::StaticClass());
	
	return Module ? Cast<AnimClass>(K2_GetModuleForClass(AnimClass::StaticClass())) : nullptr;
}


template<typename AnimType = UVoltAnimation>
FORCEINLINE AnimType* VOLT_GET_ANIMATION(TSubclassOf<UVoltAnimation> InAnimType, UObject* Owner = GetTransientPackage())
{
	return NewObject<AnimType>(Owner,InAnimType);
}
