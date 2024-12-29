//Copyright 2022~2024 DevGrain. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Misc/EngineVersionComparison.h"

#if UE_VERSION_OLDER_THAN(5,0,0)
#include "EditorStyleSet.h"
#else
#include "Styling/AppStyle.h"
#endif

class VOLTEDITOR_API FVoltEditorStyle
{
public:
	static TSharedRef<class ISlateStyle> Create();

	/** @return the singleton instance. */
	static const ISlateStyle& Get();

	static void ResetToDefault();

	static FName GetStyleSetName();

	/** @return the singleton instance. */
	static const ISlateStyle& GetUEEditorSlateStyleSet();

	static const FName GetUEEditorSlateStyleSetName();

private:
	static void SetStyleSet(const TSharedRef<class ISlateStyle>& NewStyle);

private:
	/** Singleton instances of this style. */
	static TSharedPtr<class ISlateStyle> Instance;
	
};
