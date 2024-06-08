//Copyright 2022~2024 DevGrain. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "VoltModuleItem.h"
#include "Layout/Margin.h"
#include "Volt_ASM_InterpChildSlotPadding.generated.h"

UCLASS(EditInlineNew, Blueprintable, BlueprintType)
class VOLT_API UVolt_ASM_InterpChildSlotPadding : public UVoltModuleItem
{
	
	GENERATED_BODY()

public:

	virtual void ModifySlateVariable(const float DeltaTime, const TScriptInterface<IVoltInterface>& Volt) override;

	virtual bool IsActive() override;
	
public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Module Property")
	FMargin TargetPadding;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Module Property")
	float InterpSpeed = 5;
	
};