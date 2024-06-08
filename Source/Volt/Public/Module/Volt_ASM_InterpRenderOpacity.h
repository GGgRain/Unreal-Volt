//Copyright 2022~2024 DevGrain. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "VoltModuleItem.h"
#include "UObject/NoExportTypes.h"
#include "Layout/Margin.h"
#include "Volt_ASM_InterpRenderOpacity.generated.h"

UCLASS(EditInlineNew, Blueprintable, BlueprintType)
class VOLT_API UVolt_ASM_InterpRenderOpacity : public UVoltModuleItem
{
	
	GENERATED_BODY()

public:

	virtual void ModifySlateVariable(const float DeltaTime, const TScriptInterface<IVoltInterface>& Volt) override;

	virtual bool IsActive() override;
	
public:
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Module Property")
	bool bUseStartOpacity = false; //If not specified, do nothing.
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Module Property")
	float StartOpacity; //If not specified, do nothing.
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Module Property")
	float TargetOpacity;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Module Property")
	float InterpSpeed = 5;

private:
	
	UPROPERTY(Transient)
	bool bEverUpdated = false; //If not specified, do nothing.
	
};