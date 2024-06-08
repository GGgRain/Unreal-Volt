//Copyright 2022~2024 DevGrain. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "VoltVariableActionBase.h"
#include "VoltVariableBase.h"
#include "Layout/Margin.h"
#include "Slate/WidgetTransform.h"
#include "VoltVariables.generated.h"

/**
 * Basic slate render related variable. override UVoltVariableBase to create a new variable.
 */

/**
 * A variable that affects the opacity of the slate.
 */
UCLASS(BlueprintType)
class VOLTCORE_API UVoltVar_Opacity : public UVoltVariableBase 
{

	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintReadWrite, Category="Slate - Opacity")
	float Value;
	
};

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


/**
 * A variable that affects the pivot of widget transform of the slate.
 */
UCLASS(BlueprintType)
class VOLTCORE_API UVoltVar_WidgetTransformPivot : public UVoltVariableBase 
{

	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, Category="Slate - Widget Transform Pivot")
	FVector2D Value;
	
};

/**
 * A variable that affects the color and opacity.
 */
UCLASS(BlueprintType)
class VOLTCORE_API UVoltVar_ColorAndOpacity : public UVoltVariableBase 
{

	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintReadWrite, Category="Slate - Color")
	FLinearColor Value;
};

/**
 * A variable that affects the background color of the border.
 */
UCLASS(BlueprintType)
class VOLTCORE_API UVoltVar_BackgroundColor : public UVoltVariableBase 
{

	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintReadWrite, Category="Slate - Color")
	FLinearColor Value;
	
};

/**
 * A variable that affects the padding of the slate's child slots.
 */
UCLASS(BlueprintType)
class VOLTCORE_API UVoltVar_ChildSlotPadding : public UVoltVariableBase 
{

	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, Category="Slate - Slot Padding")
	FMargin Value;
	
};

/**
 * A variable that affects the padding of the slot that contains the animated slate.
 */
UCLASS(BlueprintType)
class VOLTCORE_API UVoltVar_ParentSlotPadding : public UVoltVariableBase 
{

	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, Category="Slate - Slot Padding")
	FMargin Value;
	
};
