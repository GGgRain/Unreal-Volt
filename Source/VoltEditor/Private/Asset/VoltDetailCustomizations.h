//Copyright 2022~2024 DevGrain. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DetailWidgetRow.h"

#include "IDetailCustomization.h"

#include "Framework/Text/SlateWidgetRun.h"


class SContextTextEditor;
struct FTextRunParseResults;
class STextPropertyEditableTextBox;
class SMultiLineEditableTextBox;
class IDetailLayoutBuilder;
class UDialogueManager;

//////////////////////////////////////////////////////////////////////////
// FBoneRigDetailsCustomization

class FVoltModuleDetailCustomization: public IDetailCustomization
{
public:
	/** Makes a new instance of this detail layout class for a specific detail view requesting it */
	static TSharedRef<IDetailCustomization> MakeInstance();

	// IDetailCustomization interface
	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;
	// End of IDetailCustomization interface

public:
	
	void CreatePropertySection(IDetailLayoutBuilder& DetailBuilder);
	
};


class FVoltAnimationDetailCustomization: public IDetailCustomization
{
public:
	/** Makes a new instance of this detail layout class for a specific detail view requesting it */
	static TSharedRef<IDetailCustomization> MakeInstance();

	// IDetailCustomization interface
	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;
	// End of IDetailCustomization interface

public:
	
	void CreatePropertySection(IDetailLayoutBuilder& DetailBuilder);
	
};