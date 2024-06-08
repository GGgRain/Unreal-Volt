//Copyright 2022~2024 DevGrain. All Rights Reserved.

#include "Asset/VoltDetailCustomizations.h"

#include "BlueprintActionDatabase.h"
#include "Modules/ModuleManager.h"

#include "DetailLayoutBuilder.h"
#include "DetailCategoryBuilder.h"

#include "PropertyCustomizationHelpers.h"

#include "Kismet2/KismetEditorUtilities.h"


#define LOCTEXT_NAMESPACE "VoltDetailCustomization"

TArray<UObject*> ResolveWeakPtrObjsToRawPtrObjs(const TArray<TWeakObjectPtr<UObject>>& Objs)
{
	TArray<UObject*> ResultArr;

	ResultArr.Reserve(Objs.Num());

	for (const TWeakObjectPtr<UObject>& Object : Objs)
	{
		ResultArr.Emplace(Object.Get());
	}

	return ResultArr;
}

TSharedRef<IDetailCustomization> FVoltModuleDetailCustomization::MakeInstance()
{
	return MakeShareable(new FVoltModuleDetailCustomization);
}

void FVoltModuleDetailCustomization::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	CreatePropertySection(DetailBuilder);
}

void FVoltModuleDetailCustomization::CreatePropertySection(IDetailLayoutBuilder& DetailBuilder)
{
	const TArray<TWeakObjectPtr<UObject>>& Objs = DetailBuilder.GetSelectedObjects();

	IDetailCategoryBuilder& ModulePropertiesCategory = DetailBuilder.EditCategory("Exposed Module Properties");

	TArray<FProperty*> PropertiesToDisplay;

	for (const TWeakObjectPtr<UObject>& Object : Objs)
	{
		if (Object == nullptr) return; // Revert if there is any invalid object in array.

		for (TFieldIterator<FProperty> It(Object->GetClass()); It; ++It)
		{
			FProperty* Property = *It;

			if (Property->HasAnyPropertyFlags(CPF_DisableEditOnInstance)) continue;
			//skip if a property is Not visible (not marked as public on BP editor)

			if (!PropertiesToDisplay.Contains(Property)) PropertiesToDisplay.Add(Property);
		}
	}

	const TArray<UObject*> ResolvedObjsArr = ResolveWeakPtrObjsToRawPtrObjs(Objs);

	for (const FProperty* ToDisplay : PropertiesToDisplay)
	{
		ModulePropertiesCategory.AddExternalObjectProperty(ResolvedObjsArr, ToDisplay->GetFName());
	}
}

TSharedRef<IDetailCustomization> FVoltAnimationDetailCustomization::MakeInstance()
{
	return MakeShareable(new FVoltAnimationDetailCustomization);
}

void FVoltAnimationDetailCustomization::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	CreatePropertySection(DetailBuilder);
}

void FVoltAnimationDetailCustomization::CreatePropertySection(IDetailLayoutBuilder& DetailBuilder)
{
	const TArray<TWeakObjectPtr<UObject>>& Objs = DetailBuilder.GetSelectedObjects();

	if(Objs.Num() != 1) return; //Revert if there are more than a single animation instance to edit.

	IDetailCategoryBuilder& ModulePropertiesCategory = DetailBuilder.EditCategory("Exposed Module Properties");

	TArray<FProperty*> PropertiesToDisplay;

	for (const TWeakObjectPtr<UObject>& Object : Objs)
	{
		if (Object == nullptr) return; // Revert if there is any invalid object in array.

		for (TFieldIterator<FProperty> It(Object->GetClass()); It; ++It)
		{
			FProperty* Property = *It;

			if (Property->HasAnyPropertyFlags(CPF_DisableEditOnInstance)) continue;
			//skip if a property is Not visible (not marked as public on BP editor)

			if (!PropertiesToDisplay.Contains(Property)) PropertiesToDisplay.Add(Property);
		}
	}

	const TArray<UObject*> ResolvedObjsArr = ResolveWeakPtrObjsToRawPtrObjs(Objs);

	for (const FProperty* ToDisplay : PropertiesToDisplay)
	{
		ModulePropertiesCategory.AddExternalObjectProperty(ResolvedObjsArr, ToDisplay->GetFName());
	}
}


#undef LOCTEXT_NAMESPACE
