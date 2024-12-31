![Volt-Logo](Resources/Icon128.png)
# Volt - The Slate Architecture Animation Library for Unreal Engine
Volt is a simple, modular animation Library for Unreal Slate Architecture powered by various UObject-based modules that can manipulate slate properties as you want in both runtime and editor. 

This library is part of [Joint](https://www.unrealengine.com/marketplace/ko/product/ec432b9261c94b70a4068507d42a4f5a) Project, a powerful full-modular conversation scripting framework, especially Volt took place in the editor module to animate the slates on the editor without limitation on the production and performances. 
The project's distribution intends to enable you to use it on your editor for the visual part. Let's make Unreal's editor beautiful! 

+ 24.12.31 : Volt 1.1 has been released and a ton has been changed since v1.0! Please check out the release note: https://gggrain.github.io/Unreal-Joint-Documentation/docs/release_note/Volt/1_1_0


## Usecases
[Joint](https://www.unrealengine.com/marketplace/ko/product/ec432b9261c94b70a4068507d42a4f5a), and [Joint Native](https://github.com/GGgRain/Joint-Native) is using this plugin to animated their editor slates. Please let us know if you want to share your great project with Volt!

![2024-06-0908-47-32-ezgif com-video-to-gif-converter](https://github.com/GGgRain/Volt/assets/69423737/addadc2b-3935-4e73-9ff5-e29743c807e5)
![2024-06-0908-50-04-ezgif com-video-to-gif-converter (1)](https://github.com/GGgRain/Volt/assets/69423737/f6ace5c8-9c32-4fcb-815b-cc4b2c9b888d)
![2024-06-0908-50-04-ezgif com-video-to-gif-converter](https://github.com/GGgRain/Volt/assets/69423737/d02d3eb7-897c-419b-bd97-38177263daa4)
![2024-06-0908-47-32-ezgif com-video-to-gif-converter (1)](https://github.com/GGgRain/Volt/assets/69423737/d4085598-72a5-4771-8867-13df482f1994)
![2024-06-0908-47-32-ezgif com-video-to-gif-converter (2)](https://github.com/GGgRain/Volt/assets/69423737/7164ebe3-4d6b-4ad1-8f60-f520de2a2f0f)

Click this image to watch Volt's showcase video.
[![Showcase](https://img.youtube.com/vi/ujdCIfK_cho/0.jpg)](https://www.youtube.com/watch?v=ujdCIfK_cho)

## Installation

We assume that you are going to use Volt on your plugin.
1. Download and unzip the plugin.
2. Copy-paste the contents in the Source folder to your plugin.
3. Go to your plugin's .uplugin file and add the modules there. (Volt, VoltCore, VoltEditor). Please check out the Volt.uplugin to see the module type and loading phase.
4. And **MOST IMPORTANTLY**, **rename all the classes and module names to something unique, to prevent compatibility issues with other programs that use Volt too. Unfortunately, Epic Games forbid us to make the plugin dependent on the plugin on the marketplace so we can not share libraries between multiple projects unless it becomes a part of the engine itself.**

(Please let us know if you have any clever ideas to make it work on multiple plugins.)
## Modules

* Volt - The module that contains some basic ready-to-use animations and modules for the plugin.
* VoltCore - The module that contains the fundamental classes for the framework.

## How to use?

Please check out the header files of the classes. We provided clear instructions for each function and properties.

### Playing animations

First, you have to implement a ```UVoltAnimationManager``` that will handle all the calculations for the animation tracks for your slates.

```
if (AnimationManager == nullptr)
{
	VOLT_IMPLEMENT_MANAGER(&AnimationManager); // In header; UVoltAnimationManager* AnimationManager = nullptr;

	//To prevent GC, Animation Manager requires a consistent slate to be assigned, that actually 'has' the authority of the manager.
	AnimationManager->AssignSlate(this->AsShared());
}
```

Then you can play animations on the slates. Volt Animation Manager will automatically find or add a wrapper object for the provided slate that handles the animation data.

```
VOLT_STOP_ALL_ANIM(AnimationManager, NodeBody);

if (UVolt_ASA_Expand* Animation = VOLT_GET_ANIMATION<UVolt_ASA_Expand>(UVolt_ASA_Expand::StaticClass()))
{
	VOLT_PLAY_ANIM(GetAnimationManager(), NodeBody, Animation); // In header; TSharedPtr<SBorder> NodeBody; 
}
```

If you need, you can access the module of the animation has change its properties to manipulate the animation.


```
//Get animation object. You can also get the CDO object of animation.

if (UVolt_ASA_Emerge* Anim_Emerge = VOLT_GET_ANIMATION<UVolt_ASA_Emerge>(UVolt_ASA_Emerge::StaticClass());)
{
    //Get the opacity module to change the interpolation speed of the animation.
	if (UVolt_ASM_InterpRenderOpacity* OpacityModule = Anim_Emerge->GetModuleForClass<UVolt_ASM_InterpRenderOpacity>()) {
        OpacityModule->InterpSpeed = 10;
    }

    //Get the widget transform interpolation module and set the properties for the animation.
	if (UVolt_ASM_InterpWidgetTransform* WidgetTransformModule = Anim_Emerge->GetModuleForClass<UVolt_ASM_InterpWidgetTransform>())
	{
		WidgetTransformModule->StartWidgetTransform = FWidgetTransform(
    	FVector2D::ZeroVector,
    	FVector2D(0.9, 0.9),
    	FVector2D::ZeroVector,
    	0);
		WidgetTransformModule->InterpSpeed = 10;
	}

    //Play the animation.
	VOLT_PLAY_ANIM(NodeSlate->GetAnimationManager(), ConditionSlate, Anim_Emerge);
}
```

### Declaring animations
You can declare your own Volt Animation object, and assign some Animation Module objects on the animation that will actually manipulate the slate's variables.

Volt_ASA_Expand.h
```
//Copyright 2022~2024 DevGrain. All Rights Reserved.

#pragma once

#include "VoltAnimation.h"
#include "Volt_ASA_Expand.generated.h"

UCLASS()
class VOLT_API UVolt_ASA_Expand : public UVoltAnimation
{
public:
	
	GENERATED_BODY()

public:

	UVolt_ASA_Expand(const FObjectInitializer& ObjectInitializer);
	
};
```
Volt_ASA_Expand.cpp
```
#include "Volt_ASA_Expand.h"

#include "Module/Volt_ASM_InterpWidgetTransform.h"
#include "Slate/WidgetTransform.h"

UVolt_ASA_Expand::UVolt_ASA_Expand(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	UVolt_ASM_InterpWidgetTransform* SlotModule = ObjectInitializer.CreateDefaultSubobject<
		UVolt_ASM_InterpWidgetTransform>(this, "InterpWidgetTransform");

	Modules.Add(SlotModule);

	SlotModule->InterpSpeed = 10;
	SlotModule->TargetWidgetTransform = FWidgetTransform(
		FVector2D::ZeroVector,
		FVector2D(1.02, 1.02),
		FVector2D::ZeroVector,
		0);
}
```

### Declaring Modules

Volt_ASM_InterpWidgetTransform.h
```
#pragma once

#include "CoreMinimal.h"
#include "VoltModuleItem.h"
#include "Slate/WidgetTransform.h"
#include "Volt_ASM_InterpWidgetTransform.generated.h"

UCLASS(EditInlineNew, Blueprintable, BlueprintType)
class VOLT_API UVolt_ASM_InterpWidgetTransform : public UVoltModuleItem
{
	
	GENERATED_BODY()

public:

	virtual void ModifySlateVariable(const float DeltaTime, const TScriptInterface<IVoltInterface>& Volt) override;

	virtual bool IsActive() override;
	
public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Module Property")
	FWidgetTransform TargetWidgetTransform;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Module Property")
	bool bUseStartWidgetTransform = false; //If not specified, do nothing.
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Module Property")
	FWidgetTransform StartWidgetTransform; //If not specified, do nothing.
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Module Property")
	float InterpSpeed = 5;
	
private:
	
	UPROPERTY(Transient)
	bool bEverUpdated = false; //If not specified, do nothing.
	
};
```
Volt_ASM_InterpWidgetTransform.cpp
```
#include "Module/Volt_ASM_InterpWidgetTransform.h"

#include "VoltInterface.h"
#include "VoltVariableCollection.h"
#include "Variables/VoltVariables.h"


void UVolt_ASM_InterpWidgetTransform::ModifySlateVariable(const float DeltaTime,
                                                                const TScriptInterface<IVoltInterface>& Volt)
{
	if(Volt == nullptr) return;
	if(Volt->GetVoltVariableCollection() == nullptr) return;

	UVoltVariableBase* Var = Volt->GetVoltVariableCollection()->FindOrAddVariable(UVoltVar_WidgetTransform::StaticClass());

	UVoltVar_WidgetTransform* CastedVar = Cast<UVoltVar_WidgetTransform>(Var);

	if(!bEverUpdated && bUseStartWidgetTransform)
	{
		CastedVar->Value = StartWidgetTransform;
	}

	if(InterpSpeed > 0 )
	{
		CastedVar->Value.Angle = FMath::FInterpTo(CastedVar->Value.Angle,TargetWidgetTransform.Angle,DeltaTime,InterpSpeed);
		CastedVar->Value.Scale = FMath::Vector2DInterpTo(CastedVar->Value.Scale,TargetWidgetTransform.Scale,DeltaTime,InterpSpeed);
		CastedVar->Value.Shear = FMath::Vector2DInterpTo(CastedVar->Value.Shear,TargetWidgetTransform.Shear,DeltaTime,InterpSpeed);
		CastedVar->Value.Translation = FMath::Vector2DInterpTo(CastedVar->Value.Translation,TargetWidgetTransform.Translation,DeltaTime,InterpSpeed);
	}else
	{
		CastedVar->Value = TargetWidgetTransform;
	}
	bEverUpdated = true;
}

bool UVolt_ASM_InterpWidgetTransform::IsActive()
{

	if(!bEverUpdated) return true;
	
	const TScriptInterface<IVoltInterface>& SlateInterface = GetVoltSlate();
	if(SlateInterface == nullptr) return false;
	if(SlateInterface->GetVoltVariableCollection() == nullptr) return false;
	
	UVoltVariableBase* Var = SlateInterface->GetVoltVariableCollection()->FindOrAddVariable(UVoltVar_WidgetTransform::StaticClass());

	UVoltVar_WidgetTransform* CastedVar = Cast<UVoltVar_WidgetTransform>(Var);
	
	if(CastedVar) return CastedVar->Value != TargetWidgetTransform;

	return false;
	
}
```

Override the ```ModifySlateVariable()``` and add a logic to animate the slates. Notice we get **Volt** as a parameter, **The object unit that can be animated on Volt**. also notice we call ```Volt->GetVoltVariableCollection()``` to get the ```UVoltVariableBase``` that refers to the specific attribute of the slates we need and applying the new data there to animate the slate.

### Declaring Volt Variable & Volt Variable Action

```UVoltVariableBase``` and its children are used to contain the data for each slate attributes related to the animations and layout display. (ex, widget transform, opacity, padding etc..)

```UVoltVariableActionBase``` and its children are the objects that actually apply the variable's data to the slate.


```
/**
 * A variable that affects the widget transform of the slate.
 */
UCLASS(BlueprintType)
class VOLTCORE_API UVoltVar_WidgetTransform : public UVoltVariableBase 
{

	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, Category="Slate - Widget Transform")
	FWidgetTransform Value;

	TOptional<FSlateRenderTransform> InternalValue;
	
};
```

```
bool UVoltVarAction_ColorAndOpacity::CheckSupportWidget(TWeakPtr<SWidget> Slate)
{
	if (Slate.IsValid())
	{
		const FName SlateType = Slate.Pin()->GetType();
		if (SlateType == "SImage") return true;
		if (SlateType == "SBorder") return true;

	}

	return false;
}

void UVoltVarAction_ColorAndOpacity::ApplyVariable(UVoltVariableBase* Variable, TWeakPtr<SWidget> SlateToApply)
{
	if (!SlateToApply.IsValid()) return;

	if (!Variable) return;

	UVoltVar_ColorAndOpacity* CastedVar = Cast<UVoltVar_ColorAndOpacity>(Variable);

	if (!CastedVar) return;

	if (SlateToApply.Pin()->GetType() == "SImage")
	{
		const TSharedPtr<SImage> CastedWidget = StaticCastSharedPtr<SImage>(SlateToApply.Pin());

		CastedWidget->SetColorAndOpacity(CastedVar->Value);

		return;
	}

	if (SlateToApply.Pin()->GetType() == "SBorder")
	{
		const TSharedPtr<SBorder> CastedWidget = StaticCastSharedPtr<SBorder>(SlateToApply.Pin());

		CastedWidget->SetColorAndOpacity(CastedVar->Value);

		return;
	}
}
```

You have to make a variable and variable actions and bind them on the ```UVoltSettings``` to make that variable action to control the variable.

UVoltSettings.cpp
```
void UVoltSettings::ResetVariableActionsForVariable(const bool bSaveConfig)
{

#if WITH_EDITOR

	VariableActionsForVariable.Empty();
	
	MACRO_REGISTER_VARIABLE_ACTION(UVoltVar_Opacity::StaticClass(),UVoltVarAction_Opacity::StaticClass())
	MACRO_REGISTER_VARIABLE_ACTION(UVoltVar_WidgetTransform::StaticClass(),UVoltVarAction_WidgetTransform::StaticClass())
	MACRO_REGISTER_VARIABLE_ACTION(UVoltVar_WidgetTransformPivot::StaticClass(),UVoltVarAction_WidgetTransformPivot::StaticClass())
	MACRO_REGISTER_VARIABLE_ACTION(UVoltVar_ColorAndOpacity::StaticClass(),UVoltVarAction_ColorAndOpacity::StaticClass())
	MACRO_REGISTER_VARIABLE_ACTION(UVoltVar_BackgroundColor::StaticClass(),UVoltVarAction_BackgroundColor::StaticClass())
	MACRO_REGISTER_VARIABLE_ACTION(UVoltVar_ChildSlotPadding::StaticClass(),UVoltVarAction_ChildSlotPadding::StaticClass())
	MACRO_REGISTER_VARIABLE_ACTION(UVoltVar_ParentSlotPadding::StaticClass(),UVoltVarAction_ParentSlotPadding::StaticClass())
	
	if(bSaveConfig) SaveConfig(CPF_Config, *GetDefaultConfigFilename());

	MarkAsInitialized();
	
#endif
	
}
```

## Supports
If you have any questions related to Volt, please visit our [official discord](https://discord.gg/DzNFax2aBS)

## Licensing
This project and source codes are licensed under MIT license. Check out [LICENSE](LICENSE) for further details.
