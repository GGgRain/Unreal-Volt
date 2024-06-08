//Copyright 2022~2024 DevGrain. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

//for the ENGINE_MAJOR_VERSION and ENGINE_MINOR_VERSION macros
#include "Runtime/Launch/Resources/Version.h"

#if ENGINE_MAJOR_VERSION >= 5 && ENGINE_MINOR_VERSION >= 1
#include "Styling/AppStyle.h"
#else
#include "EditorStyleSet.h"
#endif

class FVoltEditorStyle
{
public:
	static FString InContent(const FString& RelativePath, const ANSICHAR* Extension);

	static TSharedRef<class ISlateStyle> Create();

	/** @return the singleton instance. */
	static const ISlateStyle& Get() { return *(Instance.Get()); }

	static void ResetToDefault();

	static FName GetStyleSetName();


	/** @return the singleton instance. */
	static const ISlateStyle& GetUEEditorSlateStyleSet();

	static const FName GetUEEditorSlateStyleSetName();

private:
	static void SetStyle(const TSharedRef<class ISlateStyle>& NewStyle);

private:
	/** Singleton instances of this style. */
	static TSharedPtr<class ISlateStyle> Instance;
};
